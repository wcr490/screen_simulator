#include "port_u8g2.h"

SOCKET sock = INVALID_SOCKET;

uint8_t port_init()
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
}