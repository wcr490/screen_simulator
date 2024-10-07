#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <time.h>
#include "stdint.h"

#define MSG_LEN     5
typedef uint8_t MSG_T;

int sock_init();

int connect_server(const char *addr, u_short port, SOCKET *s);
int send_msg_blocking(SOCKET s, const char *msg, char *recv_buffer, int buffer_len);
int send_msg(SOCKET s, uint8_t *buf, int len);
int sock_delete(SOCKET s);

#endif