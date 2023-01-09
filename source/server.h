#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")
int set_nb(int s) {
#ifdef _WIN32
    unsigned long nb_mode = 1;
    return ioctlsocket(s, FIONBIO, &nb_mode);
#else
    int flags = fcntl(s, F_GETFL, 0);
    if (flags < 0)
        return flags;
    flags |= O_NONBLOCK;
    return fcntl(s, F_SETFL, flags);
#endif
}

int init_socket() {
#ifdef _WIN32
    // this part is only required on Windows: it initializes the Winsock2 dll
    WSADATA wsa_data;
    if (WSAStartup(0x0202, &wsa_data)) {
        printf("unable to initialize winsock2 \n");
        return -1;
    }
#endif
    int *s = (int *)malloc(sizeof(int));
    *s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s < 0) {
        printf("unable to initialize the UDP socket \n");
        return -1;
    }
    int res = set_nb(*s);

    if (res != NOERROR) {
        printf("Error setting non blocking %d", res);
        return -1;
    }
    printf("socket %d created \n", *s);
    struct sockaddr_in sin;
    inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr);  // this will create a big endian 32 bit address
    sin.sin_family = AF_INET;
    sin.sin_port = htons(9999);  // converts 9999 to big endian
    if (bind(*s, (struct sockaddr *)&sin, sizeof(sin))) {
        printf("unable to bind the UDP socket \n");
        return -1;
    }
   
    return *s;
}
