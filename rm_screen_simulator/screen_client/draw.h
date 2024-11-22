#ifndef __DRAW_H__
#define __DRAW_H__

#include <stdint.h>
#include "client.h"

extern unsigned char ASC8X16[];
extern unsigned char ASC8X14[];

#define IMMEDIATE_FLAG 0
#define BUFFER_FLAG 1
#define UPDATE_FLAG 2
#define CLEANUP_FLAG 3
#define FILL_FLAG 4
#define CHANGE_COLOR_FLAG 5

#define XOR 2
#define WHITE 1
#define BLACK 0

int __send_msg(SOCKET s, MSG_T *msg);
void point_msg(MSG_T *msg, uint16_t x, uint16_t y);
void point_buffer_msg(MSG_T *msg, uint16_t x, uint16_t y);
void update_buffer_msg(MSG_T *msg);
void cleanup_msg(MSG_T *msg);
void change_color_msg(MSG_T *msg, uint8_t color);
void fill_msg(MSG_T *msg);

int draw_point(SOCKET s, uint16_t x, uint16_t y);
int draw_horizon_line(SOCKET s, uint16_t x, uint16_t y, uint16_t len);
int draw_vertical_line(SOCKET s, uint16_t x, uint16_t y, uint16_t len);
int draw_rectangle(SOCKET s, uint16_t x, uint16_t y, uint16_t height, uint16_t width);
int draw_char8x16(SOCKET s, char c, uint16_t x, uint16_t y);
int draw_str8x16(SOCKET s, const char *str, uint16_t x, uint16_t y, uint16_t len);
int draw_char8x14(SOCKET s, char c, uint16_t x, uint16_t y);
int draw_str8x14(SOCKET s, const char *str, uint16_t x, uint16_t y, uint16_t len);
int draw_char_arr(SOCKET s, const char *arr, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
int cleanup(SOCKET s);
int fill(SOCKET s);

int update_buffer(SOCKET s);
int change_color(SOCKET s, uint8_t color);

int draw_point_buffer(SOCKET s, uint16_t x, uint16_t y);
int draw_horizon_line_buffer(SOCKET s, uint16_t x, uint16_t y, uint16_t len);
int draw_vertical_line_buffer(SOCKET s, uint16_t x, uint16_t y, uint16_t len);
int draw_rectangle_buffer(SOCKET s, uint16_t x, uint16_t y, uint16_t height, uint16_t width);
int cleanup_buffer(SOCKET s);
int draw_char8x16_buffer(SOCKET s, char c, uint16_t x, uint16_t y);
int draw_str8x16_buffer(SOCKET s, const char *str, uint16_t x, uint16_t y, uint16_t len);
int draw_char8x14_buffer(SOCKET s, char c, uint16_t x, uint16_t y);
int draw_str8x14_buffer(SOCKET s, const char *str, uint16_t x, uint16_t y, uint16_t len);
int draw_char_arr_buffer(SOCKET s, const char *arr, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
int fill_buffer(SOCKET s);

#endif