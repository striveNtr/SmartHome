#ifndef __SOCKET__H
#define __SOCKET__H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>

#define IPADDR "192.168.1.54"
#define PORT 8192
#define BUF_SIZE 6

int socket_init(const char *ipaddr,const char *port);

//int is_disconnect(int c_fd);

#endif