#ifndef __PORT_U8G2_H__
#define __PORT_U8G2_H__

#include "client.h"
#include "draw.h"

#define unimplemented() void;

static SOCKET sock = INVALID_SOCKET;
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
#define u8g2_SetDrawColor(__unused_u8g2, color) \
    do                                          \
    {                                           \
        change_color(sock, color);              \
    } while (0)

#define u8g2_DrawRBox(__unused_u8g2, x, y, w, h, r) \
    do                                              \
    {                                               \
        unimplemented();                            \
    } while (0)

#define u8g2_DrawVLine(__unused_u8g2, x, y, len)    \
    do                                              \
    {                                               \
        draw_vertical_line_buffer(sock, x, y, len); \
    } while (0)

#define u8g2_DrawFrame(__unused_u8g2, x, y, w, h) \
    do                                       \
    {                                        \
        unimplemented(); \
    } while (0)

/*
void u8g2_ClearBuffer(u8g2_t *u8g2)
{
  size_t cnt;
  cnt = u8g2_GetU8x8(u8g2)->display_info->tile_width;
  cnt *= u8g2->tile_buf_height;
  cnt *= 8;
  memset(u8g2->tile_buf_ptr, 0, cnt);
}
*/
#define u8g2_ClearBuffer(__unused_u8g2) \
    do                                  \
    {                                   \
        cleanup_buffer(sock);           \
    } while (0)

#define u8g2_SendBuffer(__unused_u8g2) \
    do                                 \
    {                                  \
        update_buffer(sock);           \
    } while (0)

#endif