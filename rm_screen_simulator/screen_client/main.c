#include "main.h"
#include "stdio.h"

#define STR_WIDTH 8
#define STR_HEIGHT 16

// Define globally
SOCKET sock = INVALID_SOCKET;

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
    float sin = 0;
    float p = 0.1;
    float scale = 2.0f;
    int i = 0;
    while (1)
    {
        cleanup(sock);
        mat3 mat;
        /*
        float cos = newton_sqrt((1 - sin * sin) * (float)(p > 0.0f ? 1 : -1), 3);
        mat3_set_angle(&mat, sin, cos);
        */
        mat3_set_scale(&mat, scale);
        if (draw_affine_char8x14_buffer(sock, 'E', 30, 30, &mat))
        {
            return 1;
        }
        if (draw_affine_char8x14_buffer(sock, 'G', 50, 30, &mat))
        {
            return 1;
        }
        if (draw_affine_char8x14_buffer(sock, 'G', 70, 30, &mat))
        {
            return 1;
        }
        if (update_buffer(sock))
        {
            return 1;
        }
        /*
        sin += p;
        if (sin >= 1.0f || sin <= -1.0f)
            p = -p;
        printf("sin: %f, cos: %f\n", sin, cos);

        */
        if (i % 2)
        {
            scale = 0.5f;
        }
        else
        {
            scale = 2.0f;
        }
        i++;
    }

    sock_delete(sock);
    return 0;
}