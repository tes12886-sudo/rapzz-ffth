// RAPZZ arm64 ptrace injector — dlopen(librapzz.so) into a running process.
// Usage: rapzz_inject <pid> </absolute/path/librapzz.so>
// Method: hijack tracee regs to call mmap() then dlopen(), reading symbols from the
// target's on-disk libc.so so addresses match the device exactly.
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <sys/mman.h>
#include <elf.h>
#include <linux/elf.h>
#include <sys/syscall.h>

#ifndef NT_PRSTATUS
#define NT_PRSTATUS 1
#endif

#ifndef __NR_process_vm_writev
#define __NR_process_vm_writev 271
#endif

struct a64_regs {
    uint64_t regs[31];
    uint64_t sp;
    uint64_t pc;
    uint64_t pstate;
};

static int g_pid = 0;

static uintptr_t module_base(pid_t pid, const char *name, char *path_out, size_t path_sz) {
    char maps[64], line[1024];
    snprintf(maps, sizeof maps, "/proc/%d/maps", pid);
    FILE *f = fopen(maps, "r");
    if (!f) return 0;
    uintptr_t base = 0;
    while (fgets(line, sizeof line, f)) {
        if (strstr(line, name)) {
            uintptr_t start, off;
            char perms[8], path[512] = {0};
            if (sscanf(line, "%lx-%*lx %4s %lx %*s %*s %511s", &start, perms, &off, path) >= 3) {
                if (off == 0 && strstr(perms, "r")) {
                    base = start;
                    if (path_out && path[0] == '/') snprintf(path_out, path_sz, "%s", path);
                    break;
                }
            }
        }
    }
    fclose(f);
    return base;
}

// scan ELF64 .dynsym for a symbol, return its st_value (load-relative offset)
static uintptr_t symbol_offset(const char *elf_path, const char *sym) {
    int fd = open(elf_path, O_RDONLY);
    if (fd < 0) return 0;
    off_t sz = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    if (sz <= 0 || sz > (off_t)64 * 1024 * 1024) { close(fd); return 0; }
    uint8_t *buf = malloc(sz);
    if (!buf) { close(fd); return 0; }
    ssize_t rd = read(fd, buf, sz);
    close(fd);
    if (rd != sz) { free(buf); return 0; }

    Elf64_Ehdr *eh = (Elf64_Ehdr *)buf;
    if (memcmp(eh->e_ident, ELFMAG, SELFMAG) != 0) { free(buf); return 0; }
    Elf64_Shdr *sh = (Elf64_Shdr *)(buf + eh->e_shoff);
    uintptr_t result = 0;
    for (int i = 0; i < eh->e_shnum; i++) {
        if (sh[i].sh_type != SHT_DYNSYM) continue;
        Elf64_Sym *syms = (Elf64_Sym *)(buf + sh[i].sh_offset);
        const char *str = (const char *)(buf + sh[sh[i].sh_link].sh_offset);
        size_t n = sh[i].sh_size / sizeof(Elf64_Sym);
        for (size_t j = 0; j < n; j++) {
            if (syms[j].st_name && strcmp(str + syms[j].st_name, sym) == 0) {
                result = syms[j].st_value;
                break;
            }
        }
        if (result) break;
    }
    free(buf);
    return result;
}

static int ptrace_getregs(struct a64_regs *r) {
    struct iovec io = { r, sizeof(*r) };
    return ptrace(PTRACE_GETREGSET, g_pid, (void *)NT_PRSTATUS, &io);
}
static int ptrace_setregs(struct a64_regs *r) {
    struct iovec io = { r, sizeof(*r) };
    return ptrace(PTRACE_SETREGSET, g_pid, (void *)NT_PRSTATUS, &io);
}

// wait for the function-call fault (SIGSEGV at LR=0). Forward unrelated signals
// (e.g. SIGCHLD raised by pthread_create inside the .so constructor) so we don't
// mistake them for the return trap and leave the tracee stopped.
static int wait_call_stop(void) {
    int st = 0;
    for (;;) {
        if (waitpid(g_pid, &st, __WALL) < 0) return -1;
        if (!WIFSTOPPED(st)) return -1;
        int sig = WSTOPSIG(st);
        if (sig == SIGSEGV || sig == SIGTRAP || sig == SIGABRT || sig == SIGILL || sig == SIGBUS)
            return sig;
        ptrace(PTRACE_CONT, g_pid, 0, (void *)(long)sig);
    }
}

// call a function in the tracee; returns the value left in x0
static uint64_t call_func(uint64_t func, uint64_t a0, uint64_t a1, uint64_t a2,
                          uint64_t a3, uint64_t a4, uint64_t a5, struct a64_regs *orig) {
    struct a64_regs r = *orig;
    r.pc = func;
    r.regs[0] = a0; r.regs[1] = a1; r.regs[2] = a2;
    r.regs[3] = a3; r.regs[4] = a4; r.regs[5] = a5;
    r.regs[30] = 0;                 // return to 0 -> SIGSEGV we catch
    r.sp = (r.sp & ~0xFULL);        // 16-byte align
    ptrace_setregs(&r);
    ptrace(PTRACE_CONT, g_pid, 0, 0);
    int sig = wait_call_stop();
    struct a64_regs out;
    ptrace_getregs(&out);
    fprintf(stderr, "[*] call %lx -> x0=%lx (stop sig=%d)\n",
            (unsigned long)func, (unsigned long)out.regs[0], sig);
    return out.regs[0];
}

static void detach_run(void) {
    ptrace(PTRACE_CONT, g_pid, 0, 0);
    ptrace(PTRACE_DETACH, g_pid, 0, 0);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <pid> </path/librapzz.so>\n", argv[0]);
        return 1;
    }
    g_pid = atoi(argv[1]);
    const char *so = argv[2];
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    fprintf(stderr, "[*] pid=%d so=%s\n", g_pid, so);

    char libc_path[512] = {0};
    uintptr_t libc_base = module_base(g_pid, "libc.so", libc_path, sizeof libc_path);
    if (!libc_base) { fprintf(stderr, "[-] libc base not found\n"); return 1; }
    if (!libc_path[0]) snprintf(libc_path, sizeof libc_path, "/system/lib64/libc.so");
    fprintf(stderr, "[*] libc base=%lx path=%s\n", (unsigned long)libc_base, libc_path);

    uintptr_t off_mmap = symbol_offset(libc_path, "mmap");
    // dlopen lives in libdl.so on modern Android (libc.so does not export it)
    char libdl_path[512] = {0};
    uintptr_t libdl_base = module_base(g_pid, "libdl.so", libdl_path, sizeof libdl_path);
    if (!libdl_base) { fprintf(stderr, "[-] libdl base not found\n"); ptrace(PTRACE_DETACH, g_pid, 0, 0); return 1; }
    if (!libdl_path[0]) snprintf(libdl_path, sizeof libdl_path, "/system/lib64/libdl.so");
    uintptr_t off_dlopen = symbol_offset(libdl_path, "dlopen");
    if (!off_mmap || !off_dlopen) {
        fprintf(stderr, "[-] symbol resolve failed (mmap=%lx dlopen=%lx) libc=%s libdl=%s\n",
                (unsigned long)off_mmap, (unsigned long)off_dlopen, libc_path, libdl_path);
        ptrace(PTRACE_DETACH, g_pid, 0, 0);
        return 1;
    }
    uint64_t mmap_addr = libc_base + off_mmap;
    uint64_t dlopen_addr = libdl_base + off_dlopen;
    fprintf(stderr, "[*] mmap=%lx dlopen=%lx (libdl base=%lx)\n",
            (unsigned long)mmap_addr, (unsigned long)dlopen_addr, (unsigned long)libdl_base);

    if (ptrace(PTRACE_ATTACH, g_pid, 0, 0) < 0) {
        fprintf(stderr, "[-] attach failed: %s\n", strerror(errno));
        return 1;
    }
    { int st; waitpid(g_pid, &st, __WALL); }
    fprintf(stderr, "[*] attached\n");

    struct a64_regs orig;
    if (ptrace_getregs(&orig) < 0) {
        fprintf(stderr, "[-] getregs failed\n");
        detach_run();
        return 1;
    }

    // fresh RW page in the tracee
    uint64_t mem = call_func(mmap_addr, 0, 0x2000, 3 /*RW*/, 0x22 /*PRIVATE|ANON*/,
                             (uint64_t)-1, 0, &orig);
    if ((int64_t)mem <= 0) {
        fprintf(stderr, "[-] mmap in target failed: %lx\n", (unsigned long)mem);
        ptrace_setregs(&orig);
        detach_run();
        return 1;
    }

    // write the .so path into that page
    fprintf(stderr, "[*] target mmap=%lx\n", (unsigned long)mem);
    char path[512];
    snprintf(path, sizeof path, "%s", so);
    size_t plen = strlen(path) + 1;
    struct iovec local = { path, plen };
    struct iovec remote = { (void *)mem, plen };
    if (syscall(__NR_process_vm_writev, g_pid, &local, 1, &remote, 1, 0) != (ssize_t)plen) {
        fprintf(stderr, "[-] process_vm_writev failed: %s\n", strerror(errno));
        ptrace_setregs(&orig);
        detach_run();
        return 1;
    }

    // dlopen(path, RTLD_NOW)
    uint64_t handle = call_func(dlopen_addr, mem, 2 /*RTLD_NOW*/, 0, 0, 0, 0, &orig);
    fprintf(stderr, "[+] dlopen handle = %lx\n", (unsigned long)handle);

    // restore original execution state and make sure the game keeps running
    ptrace_setregs(&orig);
    detach_run();

    if (handle == 0) {
        fprintf(stderr, "[-] dlopen returned NULL (check selinux / path readability)\n");
        return 1;
    }
    printf("[RAPZZ] injected into pid %d\n", g_pid);
    return 0;
}
