#include "egg_ui.h"



void egg_switch_init(EGG_SWITCH_T *obj,short x,short y,short w,short h,short r,uint8_t onoff)
{
	obj->x = x;
	obj->y = y;
	obj->w = w;
	obj->h = h;
	obj->r = r;
	obj->onoff = onoff;
	obj->knob.r = obj->r - 2;
	obj->knob.x = obj->x + 2;
	obj->knob.y = obj->y + 2;
	obj->knob.w = obj->w / 2;
	obj->knob.h = obj->h - 4;
	obj->mask = obj->knob;
	if(onoff)
	{
		obj->mask.w = obj->x + (obj->w/2)- 2;
	}
	//egg_knob_set(&obj->knob,);
}
void egg_knob_set(EGG_KNOB_T *obj,short x,short y,short w,short h,short r)
{
	obj->x = x;
	obj->y = y;
	obj->w = w;
	obj->h = h;
	obj->r = r;
}

void egg_knob_set_wr(EGG_KNOB_T *obj,short w,short r)
{
	obj->w = w;
	obj->r = r;
}

#if 0
void egg_draw_switch(EGG_SWITCH_T *obj)
{
	//printf("x:%d,y:%d,w:%d,h:%d,r:%d,fill:%d\r\n",panel->x, panel->y, panel->w,panel->h,panel->r,panel->fill);
	if(obj->r == 0)
	{
		if(obj->onoff)
		{
			u8g2_DrawBox(&u8g2, obj->x, obj->y, obj->w,obj->h);
		}
		else
		{
			u8g2_DrawFrame(&u8g2, obj->x, obj->y, obj->w,obj->h);
		}
	}
	else
	{	
		if(!obj->onoff)
		{
			u8g2_DrawRFrame(&u8g2, obj->x, obj->y, obj->w,obj->h,obj->r);		
			u8g2_SetDrawColor(&u8g2,1);
		}
		else
		{
			u8g2_DrawRBox(&u8g2, obj->x, obj->y, obj->w,obj->h,obj->r);
			u8g2_SetDrawColor(&u8g2,0);
		}
		if(obj->anim_flag)
		{
			if(obj->onoff)
			{
				u8g2_DrawCircle(&u8g2,obj->knob.x, obj->knob.y, obj->knob.r - 1, U8G2_DRAW_ALL);
			}
			else
			{
				u8g2_DrawDisc(&u8g2,obj->knob.x  , obj->knob.y, obj->knob.r - 1, U8G2_DRAW_ALL);
			}
		}
		else
		{
			if(obj->onoff)
			{
				obj->knob.r = (obj->h/2);
				obj->knob.x = obj->x + obj->w - obj->knob.r - 1;
				obj->knob.y =  obj->y + obj->knob.r;
				u8g2_DrawCircle(&u8g2,obj->knob.x  , obj->knob.y, obj->knob.r - 1, U8G2_DRAW_ALL);
			}
			else
			{
				obj->knob.r = (obj->h/2);
				obj->knob.x = obj->x + obj->knob.r;
				obj->knob.y =  obj->y + obj->knob.r;
				u8g2_DrawDisc(&u8g2,obj->knob.x  , obj->knob.y, obj->knob.r - 1, U8G2_DRAW_ALL);
			}
		}
		u8g2_SetDrawColor(&u8g2,1);

	}
}
#else
void egg_draw_switch(EGG_SWITCH_T *obj)
{
	//printf("x:%d,y:%d,w:%d,h:%d,r:%d,fill:%d\r\n",panel->x, panel->y, panel->w,panel->h,panel->r,panel->fill);
	if(obj->r == 0)
	{
		if(obj->onoff)
		{
			u8g2_DrawBox(&u8g2, obj->x, obj->y, obj->w,obj->h);
		}
		else
		{
			u8g2_DrawFrame(&u8g2, obj->x, obj->y, obj->w,obj->h);
		}
		
		u8g2_DrawFrame(&u8g2, obj->x, obj->y, obj->w,obj->h);		
		u8g2_DrawBox(&u8g2, obj->mask.x, obj->mask.y, obj->mask.w,obj->mask.h);
	
		if(obj->anim_flag)
		{
			u8g2_SetDrawColor(&u8g2,0);
			u8g2_DrawFrame(&u8g2, obj->knob.x, obj->knob.y, obj->knob.w,obj->knob.h);
		}
		else
		{
			if(obj->onoff)
			{
				u8g2_SetDrawColor(&u8g2,0);
				u8g2_DrawFrame(&u8g2, obj->knob.x, obj->knob.y, obj->knob.w,obj->knob.h);
			}
			else
			{
				u8g2_SetDrawColor(&u8g2,0);
				u8g2_DrawFrame(&u8g2, obj->knob.x, obj->knob.y, obj->knob.w,obj->knob.h);
			}
		}
		u8g2_SetDrawColor(&u8g2,1);
	}
	else
	{	
		u8g2_DrawRFrame(&u8g2, obj->x, obj->y, obj->w,obj->h,obj->r);		
		u8g2_DrawRBox(&u8g2, obj->mask.x, obj->mask.y, obj->mask.w,obj->mask.h,obj->mask.r);
	
		if(obj->anim_flag)
		{
			u8g2_SetDrawColor(&u8g2,0);
			u8g2_DrawRFrame(&u8g2, obj->knob.x, obj->knob.y, obj->knob.w,obj->knob.h,obj->knob.r);
		}
		else
		{
			if(obj->onoff)
			{
				u8g2_SetDrawColor(&u8g2,0);
				u8g2_DrawRFrame(&u8g2, obj->knob.x, obj->knob.y, obj->knob.w,obj->knob.h,obj->knob.r);
			}
			else
			{
				u8g2_SetDrawColor(&u8g2,0);
				u8g2_DrawRFrame(&u8g2, obj->knob.x, obj->knob.y, obj->knob.w,obj->knob.h,obj->knob.r);
			}
		}
		u8g2_SetDrawColor(&u8g2,1);

	}
}

#endif
void egg_switch_set_onoff_cb(void *obj,short val)
{
	EGG_SWITCH_T *btn = (EGG_SWITCH_T *)obj;
//	printf("val:%d\r\n",val);
	btn->knob.x = val;
	btn->mask.w = val+(btn->knob.w)-3 - btn->x;
}

void egg_switch_onoff_end_cb(struct _EGG_ANIM_T_ *a)
{
	EGG_SWITCH_T *btn = (EGG_SWITCH_T *)a->obj;
	btn->anim_flag = 0;
	btn->onoff ^=1;

	//printf("end\r\n");
}

void egg_btn_onoff(EGG_SWITCH_T *obj,u8 onoff)
{
	if(obj->onoff == onoff)
	{
		return;
	}
	obj->anim_flag = 1;
	if(onoff)
	{
		//egg_anim_init(&obj->anim,obj,obj->knob.x,obj->x + obj->w - obj->knob.r - 1,150,0);
		egg_anim_init(&obj->anim,obj,obj->knob.x,obj->x + (obj->w/2),150,0);
		
	}
	else
	{
		//egg_anim_init(&obj->anim,obj,obj->knob.x,obj->x + obj->knob.r,150,0);
		egg_anim_init(&obj->anim,obj,obj->knob.x,obj->x+1,150,0);
		
	}
	egg_anim_add_set_cb(&obj->anim, egg_switch_set_onoff_cb);
	egg_anim_add_end_cb(&obj->anim, egg_switch_onoff_end_cb);
	egg_anim_add_path_cb(&obj->anim,lv_anim_path_linear);
	egg_anim_start(&obj->anim);
	
}













