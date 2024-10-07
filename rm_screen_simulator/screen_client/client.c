#include "client.h"

#pragma comment(lib, "ws2_32.lib")  // 链接 Winsock 库


int connect_server(const char *addr, u_short port, SOCKET *s) {
    if ((*s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (connect(*s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connection failed with error code : %d\n", WSAGetLastError());
        closesocket(*s);
        WSACleanup();
        return 2;
    }
    return 0;
}
int send_msg(SOCKET s, uint8_t *buf, int len) {
    if (send(s, buf, len, 0) < 0) {
        printf("Send failed : %d\n", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return 1;
    }
    printf("Message sent!\n");
    return 0;
}

int send_msg_blocking(SOCKET s, const char *msg, char *recv_buffer, int buffer_len) {
    if (send(s, msg, 1, 0) < 0) {
        printf("Send failed : %d\n", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return 1;
    }
    printf("Message sent!\n");
    memset(recv_buffer, 0, buffer_len);
    int recv_size;
    if ((recv_size = recv(s, recv_buffer, buffer_len, 0)) == SOCKET_ERROR) {
        printf("recv failed : %d\n", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return 2;
    }
    recv_buffer[recv_size] = '\0';
    return 0;
}

int sock_init() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock. Error Code : %d\n", WSAGetLastError());
        return 1;
    }
    return 0;
}
int sock_delete(SOCKET s) {
    closesocket(s);
    WSACleanup();
}