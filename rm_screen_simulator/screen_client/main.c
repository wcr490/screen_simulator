#include "main.h"
#include "stdio.h"

#define STR_WIDTH 8
#define STR_HEIGHT 16

// Define globally
SOCKET sock = INVALID_SOCKET;

int draw_STOP()
{
    if (draw_str8x16(sock, "STOP", 5 * STR_WIDTH, 0, 4))
        return 1;
}
int draw_SLOW_DOWN()
{
    if (draw_str8x16(sock, "SLOW DOWN", 3 * STR_WIDTH, 0, 9))
        return 1;
}
int draw_GO()
{
    if (draw_str8x16(sock, "GO", 6 * STR_WIDTH, 0, 2))
        return 1;
}
int main()
{
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
    // fill(sock);
    draw_GO();
    Sleep(1000);
    change_color(sock, XOR);
    fill(sock);
    while (1)
    {
    }
    
    sock_delete(sock);
}