#ifndef __PORT_U8G2_H__
#define __PORT_U8G2_H__

#include "client.h"
#include "draw.h"

extern SOCKET sock;

uint8_t port_init();
/*
U8g2 Bindings
1. Unsafe with no checking
2. No round components
3. I haven't implemented some functions like HVLink with directions(The author of U8g2 is insane)
*/

#define u8g2_DrawVLine(__unused_u8g2, x, y, len)    \
    do                                              \
    {                                               \
        draw_vertical_line_buffer(sock, x, y, len); \
    } while (0)

#define u8g2_DrawHLine(__unused_u8g2, x, y, len)   \
    do                                             \
    {                                              \
        draw_horizon_line_buffer(sock, x, y, len); \
    } while (0)

#define u8g2_SetDrawColor(__unused_u8g2, color) \
    do                                          \
    {                                           \
        change_color(sock, color);              \
    } while (0)

#define u8g2_DrawRBox(__unused_u8g2, x, y, w, h, r) u8g2_DrawBox(__unused_u8g2, x, y, w, h)

#define u8g2_DrawBox(__unused_u8g2, x, y, w, h)        \
    do                                                 \
    {                                                  \
        uint16_t ytmp = y;                             \
        uint16_t htmp = h;                             \
        while (htmp != 0)                              \
        {                                              \
            u8g2_DrawHLine(__unused_u8g2, x, ytmp, w); \
            htmp--;                                    \
            ytmp++;                                    \
        }                                              \
    } while (0)

#define u8g2_DrawRFrame(__unused_u8g2, x, y, w, h, r) u8g2_DrawFrame(__unused_u8g2, x, y, w, h)

#define u8g2_DrawFrame(__unused_u8g2, x, y, w, h)                \
    do                                                           \
    {                                                            \
        uint16_t xtmp = x;                                       \
        uint16_t ytmp = y;                                       \
        uint16_t htmp = h;                                       \
        uint16_t wtmp = w;                                       \
        u8g2_DrawHLine(__unused_u8g2, xtmp, ytmp, wtmp);         \
        if (htmp >= 2)                                           \
        {                                                        \
            htmp -= 2;                                           \
            ytmp++;                                              \
            if (htmp > 0)                                        \
            {                                                    \
                u8g2_DrawVLine(__unused_u8g2, xtmp, ytmp, htmp); \
                xtmp += wtmp;                                    \
                u8g2_DrawVLine(__unused_u8g2, xtmp, ytmp, htmp); \
                ytmp += htmp;                                    \
            }                                                    \
            u8g2_DrawHLine(__unused_u8g2, x, ytmp, wtmp);        \
        }                                                        \
    } while (0)

#define u8g2_DrawLine(__unused_u8g2, x1, y1, x2, y2)                   \
    do                                                                 \
    {                                                                  \
        draw_rectangle_buffer(sock, x1, y1, x2 - x1 + 1, y2 - y1 + 1); \
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