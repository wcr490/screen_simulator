#include "egg_ui.h"

void egg_chart_init(EGG_CHART_T *obj,short x,short y,short w,short h,short r)
{
	obj->x = x;
	obj->y = y;
	obj->w = w;
	obj->h = h;
	obj->r = r;
	obj->max = h - 2;
	obj->min = 2;
}

void egg_draw_chart(EGG_CHART_T *obj)
{
	int i;
	short *p = obj->list;

	u8g2_DrawRBox(&u8g2, obj->x, obj->y, obj->w,obj->h,obj->r);
	u8g2_SetDrawColor(&u8g2,0);  

	for(i=0; i<128; i++)
	{
     u8g2_DrawLine(&u8g2, i, p[i], i + 1, p[i + 1]);
	}
	u8g2_SetDrawColor(&u8g2,1);  

}

void egg_chart_add_val(EGG_CHART_T *obj,short val)
{
	int i;
	for(i=127;i>0; i--)
	{
		obj->list[i] = obj->list[i-1];
	}
	obj->list[0] = val;
	
}






































