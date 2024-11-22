#include "egg_ui.h"
#include "math.h"

uint32_t egg_ui_tick;
uint32_t egg_last_tick;
EGG_SCREEN_T *screen = 0;
KEY_MSG key_msg;
#if 0 
EGG_OBJECT_T object ;

uint32_t egg_tick; 

void egg_init(void)
{
	memset(&object,0,sizeof(object));
}

void egg_obj_set_parent(EGG_OBJECT_T * obj,EGG_OBJECT_T * parent)
{
	EGG_ASSERT(obj);
	EGG_ASSERT(parent);
	obj->parent = parent;
}
#if 0
void egg_obj_add_child(EGG_OBJECT_T *obj,EGG_OBJECT_T * child)
{
	EGG_ASSERT(obj);
	EGG_ASSERT(child);
	EGG_CHILD_T *child_list = (EGG_CHILD_T*)obj;
	if(child_list->cnt >= child_list->c_max)
	{
		return;
	}
	child_list->list[child_list->cnt] = child;
	child_list->cnt += 1;
}
#endif
EGG_OBJECT_T *egg_get_parent(EGG_OBJECT_T *obj)
{
	if(obj)
	{
		if(obj->parent)
		{
			return (obj->parent);
		}
	}
	return 0;
}

void egg_ui_tick(void)
{
	egg_tick++;
}

void egg_ui_run(void)
{
	
}
#endif
EGG_BASE_T egg_base = {0};
EGG_ANIM_LIST_T egg_anim_list = {0};
EGG_ANIM_T test_a;
EGG_ANIM_T test_b;

int egg_anim_add_list(EGG_ANIM_T *a)
{
	int i;

	for(i = 0;i<EGG_ANIM_MAX;i++)
	{
		if(egg_anim_list.anim[i] == 0)
		{
			break;
		}
	}
	if(i == EGG_ANIM_MAX)
	{
		return -1;
	}
	egg_anim_list.anim[i] = a;
	return i;
}

int egg_anim_del_list(EGG_ANIM_T *a)
{
	int i,flag = 0;

	for(i = 0;i<EGG_ANIM_MAX;i++)
	{
		if(egg_anim_list.anim[i] == a)
		{
			egg_anim_list.anim[i] = 0;
		//	printf("anim del %d\r\n",i);
			flag = 1;
		}
	}
	if(flag == 0)
	{
	//	printf("anim del fail\r\n");
		return -1;
	}
	return 0;
}
void egg_anim_init(EGG_ANIM_T *a,void *obj,short begin_val,short end_val,uint16_t time,uint16_t delay)
{
	if(egg_anim_add_list(a) >= 0)
	{
		memset(a,0,sizeof(EGG_ANIM_T));
		a->obj = obj;
		a->time = time;
		a->end_val = end_val;
		a->begin = 0;
		a->val = begin_val;
		a->start_val = begin_val;
		printf("a:%p obj:%p time:%d start:%d end:%d \r\n",a,obj,a->time,a->start_val,a->end_val);
		a->delay = delay;
	}
	else
	{
		printf("anim init fail!\r\n");
	}
	
}

void egg_anim_add_end_cb(EGG_ANIM_T *a,	void (*end_cb)(struct _EGG_ANIM_T_ *a))
{
	a->end_cb = end_cb;
}

void egg_anim_add_set_cb(EGG_ANIM_T *a,		void (*set_val_cb)(void *obj,short val))
{
	a->set_val_cb = set_val_cb;
}
void egg_anim_add_path_cb(EGG_ANIM_T *a,	int32_t (*path_cb)(struct _EGG_ANIM_T_ *a))
{
	a->path_cb = path_cb;
}

void egg_anim_start(EGG_ANIM_T *a)
{
	a->begin = 1;
	a->act_time = SysTick_GetTick();
}

void egg_anim_stop(EGG_ANIM_T *a)
{
	a->begin = 0;
}

int32_t lv_map(int32_t x, int32_t min_in, int32_t max_in, int32_t min_out, int32_t max_out)
{
    if(max_in >= min_in && x >= max_in) return max_out;
    if(max_in >= min_in && x <= min_in) return min_out;

    if(max_in <= min_in && x <= max_in) return max_out;
    if(max_in <= min_in && x >= min_in) return min_out;

    /**
     * The equation should be:
     *   ((x - min_in) * delta_out) / delta in) + min_out
     * To avoid rounding error reorder the operations:
     *   (x - min_in) * (delta_out / delta_min) + min_out
     */

    int32_t delta_in = max_in - min_in;
    int32_t delta_out = max_out - min_out;

    return ((x - min_in) * delta_out) / delta_in + min_out;
}
uint32_t lv_bezier3(uint32_t t, uint32_t u0, uint32_t u1, uint32_t u2, uint32_t u3)
{
    uint32_t t_rem  = 1024 - t;
    uint32_t t_rem2 = (t_rem * t_rem) >> 10;
    uint32_t t_rem3 = (t_rem2 * t_rem) >> 10;
    uint32_t t2     = (t * t) >> 10;
    uint32_t t3     = (t2 * t) >> 10;

    uint32_t v1 = (t_rem3 * u0) >> 10;
    uint32_t v2 = (3 * t_rem2 * t * u1) >> 20;
    uint32_t v3 = (3 * t_rem * t2 * u2) >> 20;
    uint32_t v4 = (t3 * u3) >> 10;

    return v1 + v2 + v3 + v4;
}


int32_t lv_anim_path_linear(EGG_ANIM_T *a)
{
		int act_time = SysTick_GetTick() - a->act_time - a->delay;
    /*Calculate the current step*/
    int32_t step = lv_map(act_time, 0, a->time, 0, LV_ANIM_RESOLUTION);

    /*Get the new value which will be proportional to `step`
     *and the `start` and `end` values*/
    int32_t new_value;
    new_value = step * (a->end_val - a->start_val);
    new_value = new_value >> LV_ANIM_RES_SHIFT;
    new_value += a->start_val;

    return new_value;
}
int32_t lv_anim_path_ease_in(EGG_ANIM_T *a)
{
		int act_time = SysTick_GetTick() - a->act_time - a->delay;
    /*Calculate the current step*/
    uint32_t t = lv_map(act_time, 0, a->time, 0, LV_BEZIER_VAL_MAX);
    int32_t step = lv_bezier3(t, 0, 50, 100, LV_BEZIER_VAL_MAX);

    int32_t new_value;
    new_value = step * (a->end_val - a->start_val);
    new_value = new_value >> LV_BEZIER_VAL_SHIFT;
    new_value += a->start_val;

    return new_value;
}

int32_t lv_anim_path_ease_out(EGG_ANIM_T *a)
{
    /*Calculate the current step*/
		int act_time = SysTick_GetTick() - a->act_time - a->delay;

    uint32_t t = lv_map(act_time, 0, a->time, 0, LV_BEZIER_VAL_MAX);
    int32_t step = lv_bezier3(t, 0, 900, 950, LV_BEZIER_VAL_MAX);

    int32_t new_value;
    new_value = step * (a->end_val - a->start_val);
    new_value = new_value >> LV_BEZIER_VAL_SHIFT;
    new_value += a->start_val;

    return new_value;
}

int32_t lv_anim_path_ease_in_out(EGG_ANIM_T *a)
{
    /*Calculate the current step*/
			int act_time = SysTick_GetTick() - a->act_time - a->delay;

    uint32_t t = lv_map(act_time, 0, a->time, 0, LV_BEZIER_VAL_MAX);
    int32_t step = lv_bezier3(t, 0, 50, 952, LV_BEZIER_VAL_MAX);

    int32_t new_value;
    new_value = step * (a->end_val - a->start_val);
    new_value = new_value >> LV_BEZIER_VAL_SHIFT;
    new_value += a->start_val;

    return new_value;
}


int32_t lv_anim_path_overshoot(EGG_ANIM_T *a)
{
    /*Calculate the current step*/
				int act_time = SysTick_GetTick() - a->act_time - a->delay;

    uint32_t t = lv_map(act_time, 0, a->time, 0, LV_BEZIER_VAL_MAX);
    int32_t step = lv_bezier3(t, 0, 1000, 1300, LV_BEZIER_VAL_MAX);

    int32_t new_value;
    new_value = step * (a->end_val - a->start_val);
    new_value = new_value >> LV_BEZIER_VAL_SHIFT;
    new_value += a->start_val;

    return new_value;
}

int32_t lv_anim_path_bounce(EGG_ANIM_T *a)
{				
	int act_time = SysTick_GetTick() - a->act_time - a->delay;

    /*Calculate the current step*/
    int32_t t = lv_map(act_time, 0, a->time, 0, LV_BEZIER_VAL_MAX);
    int32_t diff = (a->end_val - a->start_val);

    /*3 bounces has 5 parts: 3 down and 2 up. One part is t / 5 long*/

    if(t < 408) {
        /*Go down*/
        t = (t * 2500) >> LV_BEZIER_VAL_SHIFT; /*[0..1024] range*/
    }
    else if(t >= 408 && t < 614) {
        /*First bounce back*/
        t -= 408;
        t    = t * 5; /*to [0..1024] range*/
        t    = LV_BEZIER_VAL_MAX - t;
        diff = diff / 20;
    }
    else if(t >= 614 && t < 819) {
        /*Fall back*/
        t -= 614;
        t    = t * 5; /*to [0..1024] range*/
        diff = diff / 20;
    }
    else if(t >= 819 && t < 921) {
        /*Second bounce back*/
        t -= 819;
        t    = t * 10; /*to [0..1024] range*/
        t    = LV_BEZIER_VAL_MAX - t;
        diff = diff / 40;
    }
    else if(t >= 921 && t <= LV_BEZIER_VAL_MAX) {
        /*Fall back*/
        t -= 921;
        t    = t * 10; /*to [0..1024] range*/
        diff = diff / 40;
    }

    if(t > LV_BEZIER_VAL_MAX) t = LV_BEZIER_VAL_MAX;
    if(t < 0) t = 0;
    int32_t step = lv_bezier3(t, LV_BEZIER_VAL_MAX, 800, 500, 0);

    int32_t new_value;
    new_value = step * diff;
    new_value = new_value >> LV_BEZIER_VAL_SHIFT;
    new_value = a->end_val - new_value;

    return new_value;
}
#if 0
void egg_anim_run_proc(EGG_ANIM_T *a)
{
	if(a->begin)
	{
	//	printf("%p begin\r\n",a);
		if(a->count)
		{
			a->count--;
			if(a->count == 0)
			{
				a->val = a->path_cb(a);//alv_anim_path_ease_in(a);
				#if 0
				if(a->end_val > a->val)
				{
					a->val = a->val + a->add;
				}
				else if(a->end_val < a->val)
				{
					a->val = a->val - a->add;
				}
				else
#endif
									
				if(a->end_val == a->val)
				{
					if(a->end_cb)
					{
						a->end_cb(a);
					}
					a->begin = 0;
					return;
				}
			//	printf("val:%d,trg:%d\r\n",a->param->val,a->param->val_trg);
				if(a->set_val_cb)
				{
					a->set_val_cb(a->obj,a->val);
				}
				if(a->add != 1)
				{
					a->count = 1;
				}
				else
				{
					a->count = a->page_time;
				}
			}
		}
	}
}
#else
void egg_anim_run_proc(EGG_ANIM_T *a)
{
	if(a->begin)
	{
		int act_time = SysTick_GetTick() - a->act_time;
		if(act_time < a->delay)
		{
			return;
		}
		if(a->path_cb)
		{
			a->val = a->path_cb(a);
		}
		if(a->end_val == a->val)
		{
			a->begin = 0;
			egg_anim_del_list(a);
			if(a->end_cb)
			{
				a->end_cb(a);
			}
			return;
		}
		if(a->set_val_cb)
		{
			a->set_val_cb(a->obj,a->val);
		}
	}
}
#endif
void egg_anim_run(void)
{
	int i;
	
	for(i = 0;i<EGG_ANIM_MAX;i++)
	{
		if(egg_anim_list.anim[i])
		{
		//	printf("i:%p\r\n",egg_anim_list.anim[i]);
			egg_anim_run_proc(egg_anim_list.anim[i]);
		}
	}
}
EGG_PANEL_T panel1 = {0};
EGG_PANEL_T panel2 = {0};

void panel_anim_end_cb(EGG_ANIM_T *a);

void panel_x_set_cb(void *obj,short val)
{
	EGG_PANEL_T *panel = (EGG_PANEL_T *)obj;
	panel->x = val;
	panel->w = lv_map(panel->x,0,100,2,30);
	panel->h = lv_map(panel->x,0,100,2,32);
	panel->r = lv_map(panel->x,0,100,0,13);
	panel=&panel2;
	panel->x = 100-val;
	panel->w = lv_map(panel->x,0,100,2,30);
	panel->h = lv_map(panel->x,0,100,2,32);
	panel->r = lv_map(panel->x,0,100,0,13);

	//printf("x:%d w:%d\r\n",panel->x,panel->w);
	
}

void panel_anim_b_end_cb(EGG_ANIM_T *a)
{
	
	printf("anim end time:%d start:%d end:%d\r\n",SysTick_GetTick() - a->act_time,a->act_time ,SysTick_GetTick());
	
	egg_anim_init(&test_b,&panel1,panel1.x,100,1000,0);
	egg_anim_add_set_cb(&test_b, panel_x_set_cb);
	egg_anim_add_end_cb(&test_b, panel_anim_end_cb);
	egg_anim_add_path_cb(&test_b,lv_anim_path_bounce);
	egg_anim_start(&test_b);
}


void panel_anim_end_cb(EGG_ANIM_T *a)
{
	
	printf("anim end time:%d start:%d end:%d\r\n",SysTick_GetTick() - a->act_time,a->act_time ,SysTick_GetTick());
	
	egg_anim_init(&test_a,&panel1,panel1.x,0,1000,0);
	egg_anim_add_set_cb(&test_a, panel_x_set_cb);
	egg_anim_add_end_cb(&test_a, panel_anim_b_end_cb);
	egg_anim_add_path_cb(&test_a,lv_anim_path_bounce);
	egg_anim_start(&test_a);
}
EGG_SWITCH_T btn;
EGG_SWITCH_T btn1;
EGG_SWITCH_T btn2;
EGG_SWITCH_T btn3;

EGG_CHART_T chart;


void draw_test_2_init(void *p)
{
	egg_penel_init(&panel1,65, 2, 61, 28,5,0);
}
int chart_x,angle_last,angle;

void chart_draw_frame()
{
	  u8g2_SetDrawColor(&u8g2, 2);
	  void;

  u8g2_DrawRBox(&u8g2,0,0,128,32,8);
	  u8g2_SetDrawColor(&u8g2, 1);

}
uint16_t myscreen_buffer[128];

#define PI 3.1415926
void get_sin_tab1(int point, unsigned int maxnum )
{
    unsigned int i = 0, j = 0, k = 0;
    float hd = 0.0;        	//??
    float fz = 0.0;       	//??
    unsigned int tem = 0;
    j = point / 2;			//????? ???????? ???????????
    hd = PI / j;        	// p/2 ???????????
    k = maxnum / 2;      	//?????
    for( i = 0; i < point; i++ )
    {
        
        fz = k * sin( hd * i ) + k;					//i=0?sin(0)=0 ?????????,????????????  ????p/2??????????
       
 
        tem = ( unsigned int )(fz * 1.0);						//???????????????
        myscreen_buffer[i] = tem;
			egg_chart_add_val(&chart,tem);
    }
}
void draw_test_1_init(void *p)
{
	egg_chart_init(&chart,0,0,128,32,8);
	//get_sin_tab1(128,32);
}

void chart_ui_show()//chart??
{
  if(chart_x==128) chart_x=0;
//  angle=20.00+(float)analogRead(READ)/100.00;
	for(int i = 0 ;i < chart_x ;i ++)
	{
		u8g2_DrawVLine(&u8g2,i,0,myscreen_buffer[i]);
	}
  //u8g2_DrawLine(&u8g2,chart_x+11,30-(int)angle_last/2,chart_x+12,32-(int)angle/2);
  u8g2_DrawVLine(&u8g2,chart_x,0,32);
  angle_last=angle;
  chart_x+=1;
  u8g2_SetDrawColor(&u8g2,1);  
}
void draw_test_1_ui(void *p)
{
	//u8g2_DrawFrame(&u8g2, 0,0,128,32);
	//egg_draw_switch(&btn);
	//egg_draw_switch(&btn1);
	//egg_draw_switch(&btn2);
	//egg_draw_switch(&btn3);
	egg_draw_chart(&chart);

}

void draw_test_2_ui(void *p)
{
	u8g2_DrawFrame(&u8g2, 0,0,128,32);
	egg_draw_panel(&panel1);
}

void key_test_1_proc(KEY_MSG *msg)
{
	if(msg->press_flag)
	{
		msg->press_flag = 0;
		egg_chart_add_val(&chart, msg->id);

		printf("key %d press\r\n",msg->id);
	}
}

EGG_SCREEN_T egg_screen[] = 
{
	{e_UI_TEST_1,draw_test_1_init,draw_test_1_ui,key_test_1_proc},
	{e_UI_TEST_2,draw_test_2_init,draw_test_2_ui}
};

void egg_tick_cb(void)
{
	egg_ui_tick++;
}

void egg_ui_run(void)
{
	if(egg_last_tick != egg_ui_tick)
	{
		egg_anim_run();
		egg_last_tick = egg_ui_tick;
	}

	if(egg_ui_tick % 15 == 0) //200ms
	{
		int i = 0;
		int len = sizeof(egg_screen)/ sizeof(EGG_SCREEN_T);
		for(i = 0;i< len;i++)
		{
			if(egg_screen[i].id == egg_base.ui_idx)
			{
				screen = &egg_screen[i];
				if(egg_base.ui_last != egg_base.ui_idx)
				{
					egg_base.ui_last = egg_base.ui_idx;
					screen->init_cb(0);
				}
				if(screen->key_cb)
				{
					screen->key_cb(&key_msg);
				}

				u8g2_ClearBuffer(&u8g2);
				if(screen->draw_cb)
				{
					screen->draw_cb(0);
				}
				u8g2_SendBuffer(&u8g2);
			}
		}
	}
}

void egg_ui_init(void)
{
	screen = &egg_screen[0];
	screen->init_cb(0);
}
