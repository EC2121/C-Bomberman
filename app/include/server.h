#pragma once

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

int set_nb(int s);

int init_socket();
