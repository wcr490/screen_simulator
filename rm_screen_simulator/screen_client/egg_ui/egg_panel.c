#include "egg_ui.h"
#if 1

void egg_penel_init(EGG_PANEL_T *panel, short x, short y, short w, short h, short r, uint8_t fill)
{
	panel->x = x;
	panel->y = y;
	panel->w = w;
	panel->h = h;
	panel->r = r;
	panel->fill = fill;
}

void egg_draw_panel(EGG_PANEL_T *panel)
{
	printf("x:%d,y:%d,w:%d,h:%d,r:%d,fill:%d\r\n", panel->x, panel->y, panel->w, panel->h, panel->r, panel->fill);
	if (panel->r == 0)
	{
		if (panel->fill)
		{
			u8g2_DrawBox(&u8g2, panel->x, panel->y, panel->w, panel->h);
		}
		else
		{
			u8g2_DrawFrame(&u8g2, panel->x, panel->y, panel->w, panel->h);
		}
	}
	else
	{
		if (panel->fill)
		{
			u8g2_DrawRBox(&u8g2, panel->x, panel->y, panel->w, panel->h, panel->r);
		}
		else
		{
			u8g2_DrawRFrame(&u8g2, panel->x, panel->y, panel->w, panel->h, panel->r);
		}
	}
}
#endif
