#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <dlfcn.h>

int blockcount = 0;
bool Firewall1 = true;

int (*original_connect)(int, const struct sockaddr *, socklen_t);

int my_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    char ip[INET_ADDRSTRLEN];

    if (addr->sa_family == AF_INET) {
        struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
        inet_ntop(AF_INET, &addr_in->sin_addr, ip, sizeof(ip));

        if (Firewall1 &&
            (strcmp(ip, "192.168.1.1") == 0 ||
             strcmp(ip, "192.981.18.1") == 0 ||
             strcmp(ip, "1.20.541.128") == 0)) {
            printf("Connection to %s blocked\n", ip);
            blockcount++;
            return -1;
        }
    }

    struct hostent *host = gethostbyaddr(&(((struct sockaddr_in*)addr)->sin_addr), sizeof(struct in_addr), AF_INET);
    if (Firewall1 && host && 
        (strcmp(host->h_name, OBFUSCATE("dl.gmc.freefiremobile.com")) == 0 ||
         strcmp(host->h_name, OBFUSCATE("ff.dr.grtc.garenanow.com")) == 0 ||
         strcmp(host->h_name, OBFUSCATE("ff.sdk.grtc.garenanow.com")) == 0 ||
         strcmp(host->h_name, OBFUSCATE("dl.ctlin.freefiremobile.com")) == 0 ||
         strcmp(host->h_name, "dl.cvs.freefiremobile.com") == 0)) {
        printf("Connection to %s blocked\n", host->h_name);
        blockcount++;
        return -1;
    }

    return original_connect(sockfd, addr, addrlen);
}
