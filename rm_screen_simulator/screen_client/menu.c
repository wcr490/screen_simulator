#include "draw.h"
#include <stdio.h>
#include <string.h>
#include <conio.h> // Windows下使用kbhit

SOCKET sock = INVALID_SOCKET;
char *list[] =
    {
        "list_a",
        "list_b",
        "list_c",
        "list_1",
        "list_2",
        "list_3",
        "list_4",
        "list_5",
        "list_6",
};

short x, x_trg;
short y = 10, y_trg = 20;
int step_size = 16;

int ui_run(short *a, short *a_trg)
{
    if (*a < *a_trg)
    {
        *a += 2;
    }
    else if (*a > *a_trg)
    {
        *a -= 2;
    }
    else
    {
        return 0;
    }
    return 1;
}

void ui_show(void)
{
    int list_len = sizeof(list) / sizeof(list[0]);
    cleanup_buffer(sock);
    for (int i = 0; i < list_len; i++)
    {
        draw_str8x16_buffer(sock, list[i], x, y + i * step_size, strlen(list[i]));
    }
    ui_run(&y, &y_trg);
    update_buffer(sock);
}

void ui_proc(void)
{
    char command[256];

    // 检查是否有键盘输入
    if (kbhit())
    {
        fgets(command, sizeof(command), stdin); // 获取输入

        // 去除换行符
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "up") == 0)
        {
            y_trg -= step_size;
        }
        else if (strcmp(command, "down") == 0)
        {
            y_trg += step_size;
        }
        else
        {
            printf("Unknown command: %s\n", command);
        }
    }

    ui_show();
}

int main(void)
{
    if (sock_init())
    {
        printf("Failed to initialize Winsock\n");
        return 1;
    }
    if (connect_server("127.0.0.1", 8080, &sock))
    {
        printf("Failed to connect\n");
        return 2;
    }
    printf("Connected to server!\n");

    y_trg = 10;

    while (1)
    {
        ui_proc();
        Sleep(50); // 延迟以减少 CPU 使用率
    }

    return 0;
}
