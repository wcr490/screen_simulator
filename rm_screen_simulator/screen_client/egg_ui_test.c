#include "draw.h"
#include "client.h"
#include "./egg_ui/egg_ui.h"

SOCKET sock;
int main()
{
    if (port_init())
    {
        return 1;
    }
    HANDLE timer = egg_tick_init();
    if (timer == NULL)
    {
        return 1;
    }
    egg_ui_init();
    EGG_PANEL_T p;
    p.x = 0, p.y = 0, p.h = 10, p.w = 30, p.fill = 0;
    while (1)
    {
        u8g2_DrawBox(NULL, p.x, p.y, p.w, p.h);
    }
    return 0;
}