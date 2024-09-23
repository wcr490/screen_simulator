#include "main.h"

char apple[5] = {'A', 'P', 'P', 'L', 'E'};
int main()
{
    SOCKET sock = INVALID_SOCKET;
    if (sock_init())
    {
        printf("Failed to initialize Winsock");
        return 1;
    }
    if (connect_server("127.0.0.1", 8080, &sock))
    {
        printf("Failed to connect");
        return 2;
    }
    printf("Connected to server!\n");
    for (int i = 0; i < 5; i++)
    {
        if (draw_char8x16(sock, apple[i], 30 + i * 9, 20))
        {
            return 1;
        }
        Sleep(100);
    }
    Sleep(1800);
    while (1)
    {
        cleanup(sock);
        if (draw_str8x16(sock, apple, 30, 20, 5))
        {
            return 1;
        }
        MSG_T msg[5];
        update_buffer_msg(msg);
        if (__send_msg(sock, msg))
        {
            return 1;
        }
        Sleep(1800);
    }
    Sleep(400000);
    sock_delete(sock);
    return 0;
}