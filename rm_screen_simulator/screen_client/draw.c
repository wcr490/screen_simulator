#include "draw.h"

void position_to_msg(MSG_T *msg, uint16_t x, uint16_t y)
{
    // Not include command argument
    msg[1] = x >> 8;
    msg[2] = x & 0xFF;
    msg[3] = y >> 8;
    msg[4] = y & 0xFF;
}

int __send_msg(SOCKET s, MSG_T *msg)
{
    return send_msg(s, msg, 5);
}

void point_msg(MSG_T *msg, uint16_t x, uint16_t y)
{
    position_to_msg(msg, x, y);
    msg[0] = IMMEDIATE_FLAG;
}
void point_buffer_msg(MSG_T *msg, uint16_t x, uint16_t y)
{
    position_to_msg(msg, x, y);
    msg[0] = BUFFER_FLAG;
}
void update_buffer_msg(MSG_T *msg)
{
    msg[0] = UPDATE_FLAG;
}
void cleanup_msg(MSG_T *msg)
{
    msg[0] = CLEANUP_FLAG;
}
void change_color_msg(MSG_T *msg, uint8_t color)
{
    msg[0] = CHANGE_COLOR_FLAG;
    msg[1] = 0;
    msg[2] = color;
}
void fill_msg(MSG_T *msg)
{
    msg[0] = FILL_FLAG;
}

int draw_point(SOCKET s, uint16_t x, uint16_t y)
{
    MSG_T msg[5];
    point_msg(msg, x, y);
    if (__send_msg(s, msg))
    {
        return 1;
    }
    return 0;
}
int draw_horizon_line(SOCKET s, uint16_t x, uint16_t y, uint16_t len)
{
    MSG_T msg[5];
    for (int i = 0; i <= len; i++)
    {
        point_buffer_msg(msg, x + i, y);
        if (__send_msg(s, msg))
        {
            return 1;
        }
    }
    if (update_buffer(s))
    {
        return 2;
    }
    return 0;
}
int draw_vertical_line(SOCKET s, uint16_t x, uint16_t y, uint16_t len)
{
    MSG_T msg[5];
    for (int i = 0; i <= len; i++)
    {
        point_buffer_msg(msg, x, y + i);
        if (__send_msg(s, msg))
        {
            return 1;
        }
    }
    if (update_buffer(s))
    {
        return 2;
    }
    return 0;
}
int draw_rectangle(SOCKET s, uint16_t x, uint16_t y, uint16_t height, uint16_t width)
{
    MSG_T msg[5];
    for (int i = 0; i <= height; i++)
    {
        point_buffer_msg(msg, x, y + i);
        if (__send_msg(s, msg))
        {
            return 1;
        }
    }
    for (int i = 0; i <= height; i++)
    {
        point_buffer_msg(msg, x + width, y + i);
        if (__send_msg(s, msg))
        {
            return 1;
        }
    }
    for (int i = 0; i <= width; i++)
    {
        point_buffer_msg(msg, x + i, y);
        if (__send_msg(s, msg))
        {
            return 1;
        }
    }
    for (int i = 0; i <= width; i++)
    {
        point_buffer_msg(msg, x + i, y + height);
        if (__send_msg(s, msg))
        {
            return 1;
        }
    }
    if (update_buffer(s))
    {
        return 2;
    }
    return 0;
}
int draw_point_buffer(SOCKET s, uint16_t x, uint16_t y)
{
    MSG_T msg[5];
    point_buffer_msg(msg, x, y);
    if (__send_msg(s, msg))
    {
        return 1;
    }
    return 0;
}
int draw_horizon_line_buffer(SOCKET s, uint16_t x, uint16_t y, uint16_t len)
{
    MSG_T msg[5];
    for (int i = 0; i <= len; i++)
    {
        point_buffer_msg(msg, x + i, y);
        if (__send_msg(s, msg))
        {
            return 1;
        }
    }
    return 0;
}
int draw_vertical_line_buffer(SOCKET s, uint16_t x, uint16_t y, uint16_t len)
{
    MSG_T msg[5];
    for (int i = 0; i <= len; i++)
    {
        point_buffer_msg(msg, x, y + i);
        if (__send_msg(s, msg))
        {
            return 1;
        }
    }
    return 0;
}
int draw_rectangle_buffer(SOCKET s, uint16_t x, uint16_t y, uint16_t height, uint16_t width)
{
    MSG_T msg[5];
    for (int i = 0; i <= height; i++)
    {
        point_buffer_msg(msg, x, y + i);
        if (__send_msg(s, msg))
        {
            return 1;
        }
    }
    for (int i = 0; i <= height; i++)
    {
        point_buffer_msg(msg, x + width, y + i);
        if (__send_msg(s, msg))
        {
            return 1;
        }
    }
    for (int i = 0; i <= width; i++)
    {
        point_buffer_msg(msg, x + i, y);
        if (__send_msg(s, msg))
        {
            return 1;
        }
    }
    for (int i = 0; i <= width; i++)
    {
        point_buffer_msg(msg, x + i, y + height);
        if (__send_msg(s, msg))
        {
            return 1;
        }
    }

    return 0;
}

int draw_char8x16(SOCKET s, char c, uint16_t x, uint16_t y)
{
    int index = (c - ' ') * 16;
    for (int i = 0; i < 16; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            if (ASC8X16[index + i] & (128 >> k))
            {
                if (draw_point_buffer(s, x + k, y + i))
                    return 1;
            }
        }
    }
    if (update_buffer(s))
    {
        return 2;
    }
    return 0;
}
int draw_char8x16_buffer(SOCKET s, char c, uint16_t x, uint16_t y)
{
    int index = (c - ' ') * 16;
    for (int i = 0; i < 16; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            if (ASC8X16[index + i] & (128 >> k))
            {
                if (draw_point_buffer(s, x + k, y + i))
                    return 1;
            }
        }
    }
    return 0;
}

int draw_str8x16(SOCKET s, const char *str, uint16_t x, uint16_t y, uint16_t len)
{
    for (int i = 0; i < len; i++)
        if (draw_char8x16_buffer(s, str[i], x + 8 * i, y))
            return 1;
    if (update_buffer(s))
    {
        return 2;
    }

    return 0;
}
int draw_affine_char8x14_buffer(SOCKET s, char c, uint16_t x, uint16_t y, mat3 *mat)
{
    int index = (c - ' ') * 14;
    vec_xy ref_xy = {x + 4, y + 7};
    for (int i = 0; i < 14; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            if (ASC8X14[index + i] & (128 >> k))
            {
                vec_xy xy;
                xy[0] = x + k, xy[1] = y + i;
                vec_xy_transformation_apply(mat, xy, ref_xy);
                int ax = _roundf(xy[0]), ay = _roundf(xy[1]);
                if (ax < 0 || ay < 0)
                {
                    continue;
                }
                if (draw_point_buffer(s, ax, ay))
                    return 1;
            }
        }
    }
    return 0;
}
int draw_char8x14_buffer(SOCKET s, char c, uint16_t x, uint16_t y)
{
    int index = (c - ' ') * 14;
    for (int i = 0; i < 14; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            if (ASC8X14[index + i] & (128 >> k))
            {
                if (draw_point_buffer(s, x + k, y + i))
                    return 1;
            }
        }
    }
    return 0;
}
int draw_char8x14(SOCKET s, char c, uint16_t x, uint16_t y)
{
    int index = (c - ' ') * 14;
    for (int i = 0; i < 14; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            if (ASC8X14[index + i] & (128 >> k))
            {
                if (draw_point_buffer(s, x + k, y + i))
                    return 1;
            }
        }
    }
    if (update_buffer(s))
    {
        return 2;
    }
    return 0;
}

int draw_str8x16_buffer(SOCKET s, const char *str, uint16_t x, uint16_t y, uint16_t len)
{
    for (int i = 0; i < len; i++)
        if (draw_char8x16_buffer(s, str[i], x + 8 * i, y))
            return 1;
    return 0;
}
int draw_str8x14(SOCKET s, const char *str, uint16_t x, uint16_t y, uint16_t len)
{
    for (int i = 0; i < len; i++)
        if (draw_char8x14_buffer(s, str[i], x + 8 * i, y))
            return 1;
    if (update_buffer(s))
    {
        return 2;
    }
    return 0;
}

int draw_str8x14_buffer(SOCKET s, const char *str, uint16_t x, uint16_t y, uint16_t len)
{
    for (int i = 0; i < len; i++)
        if (draw_char8x14_buffer(s, str[i], x + 8 * i, y))
            return 1;
    return 0;
}
/*
    Arguement `width` must be a multiple of 8
*/
int draw_char_arr(SOCKET s, const char *arr, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    MSG_T msg[5];
    int bytes_per_row = width / 8;
    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            int byte_index = i * bytes_per_row + k / 8;
            int bit_index = 7 - (k % 8);

            if (arr[byte_index] & (1 << bit_index))
            {
                if (draw_point_buffer(s, x + k, y + i))
                    return 1;
            }
        }
    }
    if (update_buffer(s))
    {
        return 2;
    }
    return 0;
}

int draw_char_arr_buffer(SOCKET s, const char *arr, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    MSG_T msg[5];
    int bytes_per_row = width / 8;
    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            int byte_index = i * bytes_per_row + k / 8;
            int bit_index = 7 - (k % 8);

            if (arr[byte_index] & (1 << bit_index))
            {
                if (draw_point_buffer(s, x + k, y + i))
                    return 1;
            }
        }
    }
    return 0;
}
int cleanup(SOCKET s)
{
    MSG_T msg[5];
    cleanup_msg(msg);
    if (__send_msg(s, msg))
    {
        return 1;
    }
    if (update_buffer(s))
    {
        return 2;
    }
    return 0;
}
int fill(SOCKET s)
{
    MSG_T msg[5];
    fill_msg(msg);
    if (__send_msg(s, msg))
    {
        return 1;
    }
    if (update_buffer(s))
    {
        return 2;
    }
    return 0;
}
int change_color(SOCKET s, uint8_t color)
{
    MSG_T msg[5];
    change_color_msg(msg, color);
    if (__send_msg(s, msg))
    {
        return 1;
    }
    if (update_buffer(s))
    {
        return 2;
    }
    return 0;
}

int update_buffer(SOCKET s)
{
    MSG_T msg[5];
    update_buffer_msg(msg);
    if (__send_msg(s, msg))
    {
        return 1;
    }

    return 0;
}
int cleanup_buffer(SOCKET s)
{
    MSG_T msg[5];
    cleanup_msg(msg);
    if (__send_msg(s, msg))
    {
        return 1;
    }
    return 0;
}
int fill_buffer(SOCKET s)
{
    MSG_T msg[5];
    fill_msg(msg);
    if (__send_msg(s, msg))
    {
        return 1;
    }
    return 0;
}
unsigned char ASC8X16[] = // ASCII
    {
        /* [字库]：[ASC8x16E字库] [数据排列]:从左到右从上到下 [取模方式]:横向8点左高位 [正负反色]:否 [去掉重复后]共95个字符
        [总字符库]：" !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"*/

        /*-- ID:0,字符:" ",ASCII编码:20,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        /*-- ID:1,字符:"!",ASCII编码:21,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00,

        /*-- ID:2,字符:""",ASCII编码:22,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x12, 0x24, 0x24, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        /*-- ID:3,字符:"#",ASCII编码:23,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x12, 0x12, 0x12, 0x7E, 0x24, 0x24, 0x24, 0x7E, 0x24, 0x24, 0x24, 0x00, 0x00, 0x00,

        /*-- ID:4,字符:"$",ASCII编码:24,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x08, 0x3C, 0x4A, 0x4A, 0x48, 0x38, 0x0C, 0x0A, 0x0A, 0x4A, 0x4A, 0x3C, 0x08, 0x08, 0x00,

        /*-- ID:5,字符:"%",ASCII编码:25,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x44, 0xA4, 0xA8, 0xA8, 0xB0, 0x54, 0x1A, 0x2A, 0x2A, 0x4A, 0x44, 0x00, 0x00, 0x00,

        /*-- ID:6,字符:"&",ASCII编码:26,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x30, 0x48, 0x48, 0x48, 0x50, 0x6E, 0xA4, 0x94, 0x98, 0x89, 0x76, 0x00, 0x00, 0x00,

        /*-- ID:7,字符:"'",ASCII编码:27,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x60, 0x20, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        /*-- ID:8,字符:"(",ASCII编码:28,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x02, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x02, 0x00, 0x00,

        /*-- ID:9,字符:")",ASCII编码:29,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x40, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x40, 0x00, 0x00,

        /*-- ID:10,字符:"*",ASCII编码:2A,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x10, 0x10, 0xD6, 0x38, 0x38, 0xD6, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,

        /*-- ID:11,字符:"+",ASCII编码:2B,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x7F, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,

        /*-- ID:12,字符:",",ASCII编码:2C,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x20, 0x20, 0x40, 0x00,

        /*-- ID:13,字符:"-",ASCII编码:2D,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        /*-- ID:14,字符:".",ASCII编码:2E,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00,

        /*-- ID:15,字符:"/",ASCII编码:2F,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x02, 0x04, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x00, 0x00,

        /*-- ID:16,字符:"0",ASCII编码:30,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00, 0x00,

        /*-- ID:17,字符:"1",ASCII编码:31,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x08, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00, 0x00,

        /*-- ID:18,字符:"2",ASCII编码:32,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x02, 0x04, 0x08, 0x10, 0x20, 0x42, 0x7E, 0x00, 0x00, 0x00,

        /*-- ID:19,字符:"3",ASCII编码:33,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x3C, 0x42, 0x42, 0x02, 0x04, 0x18, 0x04, 0x02, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00,

        /*-- ID:20,字符:"4",ASCII编码:34,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x04, 0x0C, 0x0C, 0x14, 0x24, 0x24, 0x44, 0x7F, 0x04, 0x04, 0x1F, 0x00, 0x00, 0x00,

        /*-- ID:21,字符:"5",ASCII编码:35,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x7E, 0x40, 0x40, 0x40, 0x78, 0x44, 0x02, 0x02, 0x42, 0x44, 0x38, 0x00, 0x00, 0x00,

        /*-- ID:22,字符:"6",ASCII编码:36,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x18, 0x24, 0x40, 0x40, 0x5C, 0x62, 0x42, 0x42, 0x42, 0x22, 0x1C, 0x00, 0x00, 0x00,

        /*-- ID:23,字符:"7",ASCII编码:37,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x7E, 0x42, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00,

        /*-- ID:24,字符:"8",ASCII编码:38,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x24, 0x18, 0x24, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00,

        /*-- ID:25,字符:"9",ASCII编码:39,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x38, 0x44, 0x42, 0x42, 0x42, 0x46, 0x3A, 0x02, 0x02, 0x24, 0x18, 0x00, 0x00, 0x00,

        /*-- ID:26,字符:":",ASCII编码:3A,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,

        /*-- ID:27,字符:";",ASCII编码:3B,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x00,

        /*-- ID:28,字符:"<",ASCII编码:3C,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00, 0x00,

        /*-- ID:29,字符:"=",ASCII编码:3D,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        /*-- ID:30,字符:">",ASCII编码:3E,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00, 0x00,

        /*-- ID:31,字符:"?",ASCII编码:3F,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x3C, 0x42, 0x42, 0x62, 0x04, 0x08, 0x08, 0x08, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,

        /*-- ID:32,字符:"@",ASCII编码:40,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x38, 0x44, 0x5A, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x5C, 0x42, 0x3C, 0x00, 0x00, 0x00,

        /*-- ID:33,字符:"A",ASCII编码:41,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x10, 0x10, 0x18, 0x28, 0x28, 0x24, 0x3C, 0x44, 0x42, 0x42, 0xE7, 0x00, 0x00, 0x00,

        /*-- ID:34,字符:"B",ASCII编码:42,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xF8, 0x44, 0x44, 0x44, 0x78, 0x44, 0x42, 0x42, 0x42, 0x44, 0xF8, 0x00, 0x00, 0x00,

        /*-- ID:35,字符:"C",ASCII编码:43,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x3E, 0x42, 0x42, 0x80, 0x80, 0x80, 0x80, 0x80, 0x42, 0x44, 0x38, 0x00, 0x00, 0x00,

        /*-- ID:36,字符:"D",ASCII编码:44,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xF8, 0x44, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x44, 0xF8, 0x00, 0x00, 0x00,

        /*-- ID:37,字符:"E",ASCII编码:45,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xFC, 0x42, 0x48, 0x48, 0x78, 0x48, 0x48, 0x40, 0x42, 0x42, 0xFC, 0x00, 0x00, 0x00,

        /*-- ID:38,字符:"F",ASCII编码:46,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xFC, 0x42, 0x48, 0x48, 0x78, 0x48, 0x48, 0x40, 0x40, 0x40, 0xE0, 0x00, 0x00, 0x00,

        /*-- ID:39,字符:"G",ASCII编码:47,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x3C, 0x44, 0x44, 0x80, 0x80, 0x80, 0x8E, 0x84, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00,

        /*-- ID:40,字符:"H",ASCII编码:48,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00, 0x00,

        /*-- ID:41,字符:"I",ASCII编码:49,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00, 0x00,

        /*-- ID:42,字符:"J",ASCII编码:4A,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x88, 0xF0, 0x00,

        /*-- ID:43,字符:"K",ASCII编码:4B,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xEE, 0x44, 0x48, 0x50, 0x70, 0x50, 0x48, 0x48, 0x44, 0x44, 0xEE, 0x00, 0x00, 0x00,

        /*-- ID:44,字符:"L",ASCII编码:4C,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xE0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x42, 0xFE, 0x00, 0x00, 0x00,

        /*-- ID:45,字符:"M",ASCII编码:4D,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xEE, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x54, 0x54, 0x54, 0x54, 0xD6, 0x00, 0x00, 0x00,

        /*-- ID:46,字符:"N",ASCII编码:4E,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xC7, 0x62, 0x62, 0x52, 0x52, 0x4A, 0x4A, 0x4A, 0x46, 0x46, 0xE2, 0x00, 0x00, 0x00,

        /*-- ID:47,字符:"O",ASCII编码:4F,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x44, 0x38, 0x00, 0x00, 0x00,

        /*-- ID:48,字符:"P",ASCII编码:50,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xFC, 0x42, 0x42, 0x42, 0x42, 0x7C, 0x40, 0x40, 0x40, 0x40, 0xE0, 0x00, 0x00, 0x00,

        /*-- ID:49,字符:"Q",ASCII编码:51,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0xB2, 0x4C, 0x38, 0x06, 0x00, 0x00,

        /*-- ID:50,字符:"R",ASCII编码:52,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xFC, 0x42, 0x42, 0x42, 0x7C, 0x48, 0x48, 0x44, 0x44, 0x42, 0xE3, 0x00, 0x00, 0x00,

        /*-- ID:51,字符:"S",ASCII编码:53,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x3E, 0x42, 0x42, 0x40, 0x20, 0x18, 0x04, 0x02, 0x42, 0x42, 0x7C, 0x00, 0x00, 0x00,

        /*-- ID:52,字符:"T",ASCII编码:54,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xFE, 0x92, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00,

        /*-- ID:53,字符:"U",ASCII编码:55,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00,

        /*-- ID:54,字符:"V",ASCII编码:56,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xE7, 0x42, 0x42, 0x44, 0x24, 0x24, 0x28, 0x28, 0x18, 0x10, 0x10, 0x00, 0x00, 0x00,

        /*-- ID:55,字符:"W",ASCII编码:57,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xD6, 0x54, 0x54, 0x54, 0x54, 0x54, 0x6C, 0x28, 0x28, 0x28, 0x28, 0x00, 0x00, 0x00,

        /*-- ID:56,字符:"X",ASCII编码:58,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x18, 0x18, 0x18, 0x24, 0x24, 0x42, 0xE7, 0x00, 0x00, 0x00,

        /*-- ID:57,字符:"Y",ASCII编码:59,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0xEE, 0x44, 0x44, 0x28, 0x28, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00,

        /*-- ID:58,字符:"Z",ASCII编码:5A,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x7E, 0x84, 0x04, 0x08, 0x08, 0x10, 0x20, 0x20, 0x42, 0x42, 0xFC, 0x00, 0x00, 0x00,

        /*-- ID:59,字符:"[",ASCII编码:5B,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x1E, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1E, 0x00, 0x00,

        /*-- ID:60,字符:"\",ASCII编码:5C,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x40, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x04, 0x02, 0x02, 0x00,

        /*-- ID:61,字符:"]",ASCII编码:5D,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x78, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x78, 0x00, 0x00,

        /*-- ID:62,字符:"^",ASCII编码:5E,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x18, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        /*-- ID:63,字符:"_",ASCII编码:5F,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00,

        /*-- ID:64,字符:"`",ASCII编码:60,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x60, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        /*-- ID:65,字符:"a",ASCII编码:61,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x0C, 0x34, 0x44, 0x4C, 0x36, 0x00, 0x00, 0x00,

        /*-- ID:66,字符:"b",ASCII编码:62,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0xC0, 0x40, 0x40, 0x58, 0x64, 0x42, 0x42, 0x42, 0x64, 0x58, 0x00, 0x00, 0x00,

        /*-- ID:67,字符:"c",ASCII编码:63,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x40, 0x40, 0x40, 0x22, 0x1C, 0x00, 0x00, 0x00,

        /*-- ID:68,字符:"d",ASCII编码:64,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x06, 0x02, 0x02, 0x3E, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3B, 0x00, 0x00, 0x00,

        /*-- ID:69,字符:"e",ASCII编码:65,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x7E, 0x40, 0x42, 0x3C, 0x00, 0x00, 0x00,

        /*-- ID:70,字符:"f",ASCII编码:66,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x0C, 0x12, 0x10, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00, 0x00,

        /*-- ID:71,字符:"g",ASCII编码:67,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x44, 0x44, 0x38, 0x40, 0x3C, 0x42, 0x42, 0x3C, 0x00,

        /*-- ID:72,字符:"h",ASCII编码:68,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0xC0, 0x40, 0x40, 0x5C, 0x62, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00, 0x00,

        /*-- ID:73,字符:"i",ASCII编码:69,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00, 0x00,

        /*-- ID:74,字符:"j",ASCII编码:6A,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x1C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x44, 0x78, 0x00,

        /*-- ID:75,字符:"k",ASCII编码:6B,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0xC0, 0x40, 0x40, 0x4E, 0x48, 0x50, 0x70, 0x48, 0x44, 0xEE, 0x00, 0x00, 0x00,

        /*-- ID:76,字符:"l",ASCII编码:6C,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x10, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00, 0x00,

        /*-- ID:77,字符:"m",ASCII编码:6D,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x49, 0x49, 0x49, 0x49, 0x49, 0xED, 0x00, 0x00, 0x00,

        /*-- ID:78,字符:"n",ASCII编码:6E,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x62, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00, 0x00,

        /*-- ID:79,字符:"o",ASCII编码:6F,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00,

        /*-- ID:80,字符:"p",ASCII编码:70,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD8, 0x64, 0x42, 0x42, 0x42, 0x64, 0x58, 0x40, 0xE0, 0x00,

        /*-- ID:81,字符:"q",ASCII编码:71,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1A, 0x26, 0x42, 0x42, 0x42, 0x26, 0x1A, 0x02, 0x07, 0x00,

        /*-- ID:82,字符:"r",ASCII编码:72,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x32, 0x20, 0x20, 0x20, 0x20, 0xF8, 0x00, 0x00, 0x00,

        /*-- ID:83,字符:"s",ASCII编码:73,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x42, 0x40, 0x3C, 0x02, 0x42, 0x7C, 0x00, 0x00, 0x00,

        /*-- ID:84,字符:"t",ASCII编码:74,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x12, 0x0C, 0x00, 0x00, 0x00,

        /*-- ID:85,字符:"u",ASCII编码:75,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3B, 0x00, 0x00, 0x00,

        /*-- ID:86,字符:"v",ASCII编码:76,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x44, 0x44, 0x28, 0x28, 0x10, 0x10, 0x00, 0x00, 0x00,

        /*-- ID:87,字符:"w",ASCII编码:77,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDB, 0x89, 0x4A, 0x5A, 0x54, 0x24, 0x24, 0x00, 0x00, 0x00,

        /*-- ID:88,字符:"x",ASCII编码:78,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x24, 0x18, 0x18, 0x18, 0x24, 0x6E, 0x00, 0x00, 0x00,

        /*-- ID:89,字符:"y",ASCII编码:79,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x18, 0x18, 0x10, 0x10, 0x60, 0x00,

        /*-- ID:90,字符:"z",ASCII编码:7A,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x44, 0x08, 0x10, 0x10, 0x22, 0x7E, 0x00, 0x00, 0x00,

        /*-- ID:91,字符:"{",ASCII编码:7B,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x03, 0x00, 0x00,

        /*-- ID:92,字符:"|",ASCII编码:7C,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x80,

        /*-- ID:93,字符:"}",ASCII编码:7D,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0xC0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0xC0, 0x00, 0x00,

        /*-- ID:94,字符:"~",ASCII编码:7E,对应字:宽x高=8x16,画布:宽W=8 高H=16,共16字节*/
        0x5A, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

};
unsigned char ASC8X14[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // space 0x20
    0x00, 0x00, 0x18, 0x3C, 0x3C, 0x3C, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, // !
    0x00, 0x63, 0x63, 0x63, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // "
    0x00, 0x00, 0x00, 0x36, 0x36, 0x7F, 0x36, 0x36, 0x36, 0x7F, 0x36, 0x36, 0x00, 0x00, // #
    0x0C, 0x0C, 0x3E, 0x63, 0x61, 0x60, 0x3E, 0x03, 0x03, 0x43, 0x63, 0x3E, 0x0C, 0x0C, // $
    0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x63, 0x06, 0x0C, 0x18, 0x33, 0x63, 0x00, 0x00, // %
    0x00, 0x00, 0x00, 0x1C, 0x36, 0x36, 0x1C, 0x3B, 0x6E, 0x66, 0x66, 0x3B, 0x00, 0x00, // &
    0x00, 0x30, 0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // '
    0x00, 0x00, 0x0C, 0x18, 0x18, 0x30, 0x30, 0x30, 0x30, 0x18, 0x18, 0x0C, 0x00, 0x00, // (
    0x00, 0x00, 0x18, 0x0C, 0x0C, 0x06, 0x06, 0x06, 0x06, 0x0C, 0x0C, 0x18, 0x00, 0x00, // )
    0x00, 0x00, 0x00, 0x00, 0x42, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x42, 0x00, 0x00, 0x00, // *
    0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, // +
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x30, // ,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // -
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, // .
    0x00, 0x00, 0x01, 0x03, 0x07, 0x0E, 0x1C, 0x38, 0x70, 0xE0, 0xC0, 0x80, 0x00, 0x00, // / (forward slash)
    0x00, 0x00, 0x3E, 0x63, 0x63, 0x63, 0x6B, 0x6B, 0x63, 0x63, 0x63, 0x3E, 0x00, 0x00, // 0 0x30
    0x00, 0x00, 0x0C, 0x1C, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00, 0x00, // 1
    0x00, 0x00, 0x3E, 0x63, 0x03, 0x06, 0x0C, 0x18, 0x30, 0x61, 0x63, 0x7F, 0x00, 0x00, // 2
    0x00, 0x00, 0x3E, 0x63, 0x03, 0x03, 0x1E, 0x03, 0x03, 0x03, 0x63, 0x3E, 0x00, 0x00, // 3
    0x00, 0x00, 0x06, 0x0E, 0x1E, 0x36, 0x66, 0x66, 0x7F, 0x06, 0x06, 0x0F, 0x00, 0x00, // 4
    0x00, 0x00, 0x7F, 0x60, 0x60, 0x60, 0x7E, 0x03, 0x03, 0x63, 0x73, 0x3E, 0x00, 0x00, // 5
    0x00, 0x00, 0x1C, 0x30, 0x60, 0x60, 0x7E, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00, 0x00, // 6
    0x00, 0x00, 0x7F, 0x63, 0x03, 0x06, 0x06, 0x0C, 0x0C, 0x18, 0x18, 0x18, 0x00, 0x00, // 7
    0x00, 0x00, 0x3E, 0x63, 0x63, 0x63, 0x3E, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00, 0x00, // 8
    0x00, 0x00, 0x3E, 0x63, 0x63, 0x63, 0x63, 0x3F, 0x03, 0x03, 0x06, 0x3C, 0x00, 0x00, // 9
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, // :
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x30, // ;
    0x00, 0x00, 0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x00, 0x00, // <
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, // =
    0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x00, 0x00, // >
    0x00, 0x00, 0x3E, 0x63, 0x63, 0x06, 0x0C, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x00, 0x00, // ?
    0x00, 0x00, 0x3E, 0x63, 0x63, 0x6F, 0x6B, 0x6B, 0x6E, 0x60, 0x60, 0x3E, 0x00, 0x00, // @ 0x40
    0x00, 0x00, 0x08, 0x1C, 0x36, 0x63, 0x63, 0x63, 0x7F, 0x63, 0x63, 0x63, 0x00, 0x00, // A
    0x00, 0x00, 0x7E, 0x33, 0x33, 0x33, 0x3E, 0x33, 0x33, 0x33, 0x33, 0x7E, 0x00, 0x00, // B
    0x00, 0x00, 0x1E, 0x33, 0x61, 0x60, 0x60, 0x60, 0x60, 0x61, 0x33, 0x1E, 0x00, 0x00, // C
    0x00, 0x00, 0x7C, 0x36, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x36, 0x7C, 0x00, 0x00, // D
    0x00, 0x00, 0x7F, 0x33, 0x31, 0x34, 0x3C, 0x34, 0x30, 0x31, 0x33, 0x7F, 0x00, 0x00, // E
    0x00, 0x00, 0x7F, 0x33, 0x31, 0x34, 0x3C, 0x34, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00, // F
    0x00, 0x00, 0x1E, 0x33, 0x61, 0x60, 0x60, 0x6F, 0x63, 0x63, 0x37, 0x1D, 0x00, 0x00, // G
    0x00, 0x00, 0x63, 0x63, 0x63, 0x63, 0x7F, 0x63, 0x63, 0x63, 0x63, 0x63, 0x00, 0x00, // H
    0x00, 0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, // I
    0x00, 0x00, 0x0F, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3C, 0x00, 0x00, // J
    0x00, 0x00, 0x73, 0x33, 0x36, 0x36, 0x3C, 0x36, 0x36, 0x33, 0x33, 0x73, 0x00, 0x00, // K
    0x00, 0x00, 0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x33, 0x7F, 0x00, 0x00, // L
    0x00, 0x00, 0x63, 0x77, 0x7F, 0x6B, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x00, 0x00, // M
    0x00, 0x00, 0x63, 0x63, 0x73, 0x7B, 0x7F, 0x6F, 0x67, 0x63, 0x63, 0x63, 0x00, 0x00, // N
    0x00, 0x00, 0x1C, 0x36, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00, 0x00, // O
    0x00, 0x00, 0x7E, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00, // P 0x50
    0x00, 0x00, 0x3E, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x6B, 0x6F, 0x3E, 0x06, 0x07, // Q
    0x00, 0x00, 0x7E, 0x33, 0x33, 0x33, 0x3E, 0x36, 0x36, 0x33, 0x33, 0x73, 0x00, 0x00, // R
    0x00, 0x00, 0x3E, 0x63, 0x63, 0x30, 0x1C, 0x06, 0x03, 0x63, 0x63, 0x3E, 0x00, 0x00, // S
    0x00, 0x00, 0xFF, 0xDB, 0x99, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, // T
    0x00, 0x00, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00, 0x00, // U
    0x00, 0x00, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x08, 0x00, 0x00, // V
    0x00, 0x00, 0x63, 0x63, 0x63, 0x63, 0x63, 0x6B, 0x6B, 0x7F, 0x36, 0x36, 0x00, 0x00, // W
    0x00, 0x00, 0xC3, 0xC3, 0x66, 0x3C, 0x18, 0x18, 0x3C, 0x66, 0xC3, 0xC3, 0x00, 0x00, // X
    0x00, 0x00, 0xC3, 0xC3, 0xC3, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, // Y
    0x00, 0x00, 0x7F, 0x63, 0x43, 0x06, 0x0C, 0x18, 0x30, 0x61, 0x63, 0x7F, 0x00, 0x00, // Z
    0x00, 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00, 0x00, // [
    0x00, 0x00, 0x80, 0xC0, 0xE0, 0x70, 0x38, 0x1C, 0x0E, 0x07, 0x03, 0x01, 0x00, 0x00, // \ (back slash)
    0x00, 0x00, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00, 0x00, // ]
    0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ^
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, // _
    0x18, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ` 0x60
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x46, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00, 0x00, // a
    0x00, 0x00, 0x70, 0x30, 0x30, 0x3C, 0x36, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00, 0x00, // b
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x63, 0x60, 0x60, 0x60, 0x63, 0x3E, 0x00, 0x00, // c
    0x00, 0x00, 0x0E, 0x06, 0x06, 0x1E, 0x36, 0x66, 0x66, 0x66, 0x66, 0x3B, 0x00, 0x00, // d
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x63, 0x63, 0x7E, 0x60, 0x63, 0x3E, 0x00, 0x00, // e
    0x00, 0x00, 0x1C, 0x36, 0x32, 0x30, 0x7C, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00, // f
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x66, 0x3C, // g
    0x00, 0x00, 0x70, 0x30, 0x30, 0x36, 0x3B, 0x33, 0x33, 0x33, 0x33, 0x73, 0x00, 0x00, // h
    0x00, 0x00, 0x0C, 0x0C, 0x00, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00, // i
    0x00, 0x00, 0x06, 0x06, 0x00, 0x0E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3C, // j
    0x00, 0x00, 0x70, 0x30, 0x30, 0x33, 0x33, 0x36, 0x3C, 0x36, 0x33, 0x73, 0x00, 0x00, // k
    0x00, 0x00, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00, // l
    0x00, 0x00, 0x00, 0x00, 0x00, 0x6E, 0x7F, 0x6B, 0x6B, 0x6B, 0x6B, 0x6B, 0x00, 0x00, // m
    0x00, 0x00, 0x00, 0x00, 0x00, 0x6E, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00, // n
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00, 0x00, // o
    0x00, 0x00, 0x00, 0x00, 0x00, 0x6E, 0x33, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x30, 0x78, // p 0x70
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, // q
    0x00, 0x00, 0x00, 0x00, 0x00, 0x6E, 0x3B, 0x33, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00, // r
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x63, 0x38, 0x0E, 0x03, 0x63, 0x3E, 0x00, 0x00, // s
    0x00, 0x00, 0x08, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x1B, 0x0E, 0x00, 0x00, // t
    0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3B, 0x00, 0x00, // u
    0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x36, 0x36, 0x1C, 0x1C, 0x08, 0x00, 0x00, // v
    0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x63, 0x6B, 0x6B, 0x7F, 0x36, 0x00, 0x00, // w
    0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x36, 0x1C, 0x1C, 0x1C, 0x36, 0x63, 0x00, 0x00, // x
    0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3F, 0x03, 0x06, 0x3C, // y
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x66, 0x0C, 0x18, 0x30, 0x63, 0x7F, 0x00, 0x00, // z
    0x00, 0x00, 0x0E, 0x18, 0x18, 0x18, 0x70, 0x18, 0x18, 0x18, 0x18, 0x0E, 0x00, 0x00, // {
    0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, // |
    0x00, 0x00, 0x70, 0x18, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x18, 0x18, 0x70, 0x00, 0x00, // }
    0x00, 0x00, 0x3B, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ~
    0x00, 0x70, 0xD8, 0xD8, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // DEL
};