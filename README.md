# RAPZZ — Free Fire TH (com.dts.freefireth)

Port of the ffadv (Free Fire MAX) native ImGui cheat to **com.dts.freefireth**, resolved
against the freefireth `dump.cs`. No Frida, no online login. Credit: **RAPZZ**.

## Features

- Native ImGui menu drawn inside `eglSwapBuffers` (5 tabs: AIM / ESP / RAGE / VISUAL / UI STYLE)
- **ESP**: box, name, distance, health bar, snapline, enemy count, FOV circle
- **Aimbot**: rage + legit smoothing, FOV limit, head/neck/body/hip target, fire/scope/rush triggers
- **Visual**: wallhack/chams (`_AlphaMask` shader hook) + wireframe via `glDrawElements` hook
- **Report/ban suppression**: `COW.EventLogger`, `GameRegionSwitch`, `UIModelGroup`,
  `UIModelRecruit`, `BanReasonDataManager` patched (all verified present in the TH dump)
- Theme colors, dark/light mode, 3 toggle styles, 5-finger gesture to show/hide menu
- `JNI_OnLoad` boot thread waits for `libil2cpp.so`, resolves everything by-name via il2cpp exports

### AimKill (RAGE tab) — caveat

The aimkill/damage-send module is ported structurally and **guarded**. The MAX-only weapon
internals (`HBIBDMMOOOK` field/method names, object pool, damage pack) are fully obfuscated on
TH and cannot be mapped by name without the MAX dump for cross-reference. `AimKillOffsetsReady()`
bails out cleanly if any offset fails to resolve, so it never crashes — it just stays inert until
those TH offsets are confirmed. ESP / Aimbot / Visual do not depend on it.

## Architecture

- `jni/ZygModule/cheat.cpp` — boot thread, guarded patches, `UpdateBehavior` + `eglSwapBuffers` hooks
- `jni/Hush/updates.h` — il2cpp by-name resolvers; TH match class `JMAGGLCNGIG`, player dictionaries
  `NOLEDCGOJKC / NJCGFLANJLH / GOFLDILNLBA / KAENAIKJLDD`
- `jni/Hush/ESP.h`, `aimbot.h`, `Chams.h`, `Madrchod~Aimkill.h` — features
- `jni/ZygModule/layout.h` — iOS-style menu (RAPZZ branding)
- `jni/Misc/update/Il2Cpp.cpp` — resolver (log tag `RAPZZ`), `xdl` for symbol lookup
- `jni/Misc/imgui/` — ImGui 1.89 + OpenGL3/Android backends
- `jni/KittyMemory/` — `MemoryPatch` for inline hex patches
- `jni/ZygModule/And64InlineHook.cpp` — arm64 inline hook

## Build (Windows, NDK r27)

```powershell
powershell -ExecutionPolicy Bypass -File rapzz\tools\build_native.ps1
```

Uses `C:\Android\android-sdk\ndk\27.3.13750724` clang directly. C and C++ compiled separately,
linked with `-static-libstdc++` (no `libc++_shared.so` needed). Output: `rapzz/build/librapzz.so`
(arm64-v8a, bionic SONAMEs only).

## Re-patch a base APK

```powershell
# 1. decode base APK
java -jar tools\apktool.jar d extracted\com.dts.freefireth.apk -o work\apk_base_smali -f
# 2. inject loader (see work\apk_base_smali\smali\com\rapzz\RapzzLoader.smali)
#    FFApplication.onCreate -> invoke-static {p0}, Lcom/rapzz/RapzzLoader;->start(Landroid/content/Context;)V
# 3. rebuild + extract classes.dex
java -jar tools\apktool.jar b work\apk_base_smali -o work\rebuilt_rapzz.apk -f
#    (extract classes.dex from rebuilt_rapzz.apk -> rapzz\build\classes_patched.dex)
# 4. patch
python rapzz\tools\patcher.py extracted\com.dts.freefireth.apk rapzz\build\classes_patched.dex rapzz\build\librapzz.so rapzz\build\base_rapzz.apk
# 5. zipalign + sign (same key for ALL splits)
```

## Install (ready-signed set in rapzz/build/)

All three APKs are signed with the same key (`rapzz.keystore`, store/key pass `rapzz123`, alias `rapzz`):

- `com.dts.freefireth_rapzz.apk` (patched base)
- `config.arm64_v8a_rapzz.apk`
- `asset_pack_install_time_rapzz.apk`

```powershell
adb install-multiple rapzz\build\com.dts.freefireth_rapzz.apk rapzz\build\config.arm64_v8a_rapzz.apk rapzz\build\asset_pack_install_time_rapzz.apk
```

Or install the three together via MT Manager / SAI (split APK installer). Launch the game,
the menu auto-opens; 5-finger tap toggles it.

## Root injection (no APK repack) — `/data/local/tmp/ffcheat.sh`

Instead of patching the APK, inject `librapzz.so` into the running game with the bundled
arm64 ptrace injector. The `.so` self-starts from a constructor, so a plain `dlopen` is enough
(no `System.loadLibrary` needed).

Files (all in `rapzz/build/`): `librapzz.so`, `rapzz_inject` (static arm64), `ffcheat.sh`.

```powershell
# one-shot from PC (adb + Magisk root):
powershell -ExecutionPolicy Bypass -File rapzz\tools\push_and_run.ps1
```

Or manually on device:

```sh
adb push rapzz/build/librapzz.so  /data/local/tmp/
adb push rapzz/build/rapzz_inject /data/local/tmp/
adb push rapzz/build/ffcheat.sh   /data/local/tmp/
adb shell
su
sh /data/local/tmp/ffcheat.sh
```

`ffcheat.sh` self-elevates, stages the `.so` into `/data/data/com.dts.freefireth/rapzz/`
(app-readable, selinux-safe), launches the game if needed, finds the PID and calls
`rapzz_inject <pid> <lib>`. The injector resolves `mmap`/`dlopen` from the target's on-disk
`libc.so`, allocates a page in the tracee, writes the path and calls `dlopen(path, RTLD_NOW)`.

Rebuild the injector: `clang --target=aarch64-linux-android21 -static -O2 rapzz/tools/injector.c -o rapzz/build/rapzz_inject`.

If `dlopen` returns NULL: wait until the game reaches the lobby and re-run, or check Magisk
SELinux is permissive enough for the app to read its own data dir.

## Notes

- Target ABI: arm64-v8a only
- If you re-sign with your own key, sign **every** split with that same key
- Offsets resolve by-name at runtime, so minor game updates that keep class/method names still work;
  obfuscated-name changes (match class, dictionaries) need a fresh dump.cs re-map in `updates.h`
