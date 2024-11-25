#ifndef _EGG_UI_H_
#define _EGG_UI_H_

/*
#include "main.h"
#include "oled.h"
*/

#include "port.h"

#include "stdint.h"
#define u8 uint8_t
#define u32 uint32_t

typedef struct
{
	uint8_t id;
	uint8_t val;
	uint8_t long_flag;
	uint8_t press_flag;
} KEY_MSG;

enum
{
	e_UI_TEST_1,
	e_UI_TEST_2,
	e_UI_MAX,
};

// -----------------------base------------------------//
typedef struct _EGG_SCREEN_T_
{
	uint32_t id;
	void (*init_cb)(void *p);
	void (*draw_cb)(void *p);
	void (*key_cb)(KEY_MSG *);
	struct _EGG_SCREEN_T_ *last_screen;
	struct _EGG_SCREEN_T_ *next_screen;
	short x;
	short y;

} EGG_SCREEN_T;
typedef struct
{
	short x;
	short y;
	short w;
	short h;
	short r;
	short dis;
} EGG_KNOB_T;
typedef struct
{
	uint8_t ui_idx;
	uint8_t ui_last;
	uint8_t res[2];
} EGG_BASE_T;
// -----------------------anim------------------------//
#define LV_ANIM_RES_SHIFT 10
#define LV_ANIM_RESOLUTION 1024
#define LV_BEZIER_VAL_MAX 1024 /**< Max time in Bezier functions (not [0..1] to use integers)*/
#define LV_BEZIER_VAL_SHIFT 10 /**< log2(LV_BEZIER_VAL_MAX): used to normalize up scaled values*/

#define EGG_ANIM_MAX 10

typedef struct _EGG_ANIM_T_
{
	// EGG_ANIM_VAL_T *param;
	void *obj;
	uint16_t time; // ms
	uint16_t delay;
	u32 act_time;
	uint8_t begin;
	uint8_t res[3];
	short val;
	short start_val;
	short end_val;
	// uint8_t res[2];
	void (*set_val_cb)(void *obj, short val);
	void (*end_cb)(struct _EGG_ANIM_T_ *a);
	int32_t (*path_cb)(struct _EGG_ANIM_T_ *a);
} EGG_ANIM_T;

typedef struct
{
	EGG_ANIM_T *anim[EGG_ANIM_MAX];
} EGG_ANIM_LIST_T;

int32_t lv_anim_path_linear(EGG_ANIM_T *a);
int32_t lv_anim_path_ease_in(EGG_ANIM_T *a);
int32_t lv_anim_path_ease_out(EGG_ANIM_T *a);
int32_t lv_anim_path_ease_in_out(EGG_ANIM_T *a);
int32_t lv_anim_path_overshoot(EGG_ANIM_T *a);
int32_t lv_anim_path_bounce(EGG_ANIM_T *a);
void egg_anim_init(EGG_ANIM_T *a, void *obj, short begin_val, short end_val, uint16_t time, uint16_t delay);
void egg_anim_add_end_cb(EGG_ANIM_T *a, void (*end_cb)(struct _EGG_ANIM_T_ *a));
void egg_anim_add_set_cb(EGG_ANIM_T *a, void (*set_val_cb)(void *obj, short val));
void egg_anim_add_path_cb(EGG_ANIM_T *a, int32_t (*path_cb)(struct _EGG_ANIM_T_ *a));
void egg_anim_start(EGG_ANIM_T *a);
void egg_anim_stop(EGG_ANIM_T *a);

// -----------------------panel------------------------//
typedef struct
{
	short x;
	short y;
	short w;
	short h;
	short r;
	u8 fill;
	u8 res[1];
} EGG_PANEL_T;

void egg_draw_panel(EGG_PANEL_T *panel);
void egg_penel_init(EGG_PANEL_T *panel, short x, short y, short w, short h, short r, uint8_t fill);

// -----------------------switch------------------------//
typedef struct
{
	short x;
	short y;
	short w;
	short h;
	short r;
	u8 onoff;
	u8 anim_flag;
	EGG_KNOB_T knob;
	EGG_KNOB_T mask;
	EGG_ANIM_T anim;
} EGG_SWITCH_T;
void egg_switch_init(EGG_SWITCH_T *obj, short x, short y, short w, short h, short r, uint8_t onoff);
void egg_draw_switch(EGG_SWITCH_T *obj);
void egg_btn_onoff(EGG_SWITCH_T *obj, u8 onoff);
void egg_knob_set_wr(EGG_KNOB_T *obj, short w, short r);

// -----------------------chart------------------------//
#define EGG_CHART_POINT_MAX 128

typedef struct
{
	short x;
	short y;

	short w;
	short h;

	short r;
	short max;

	short min;
	short res;

	short list[EGG_CHART_POINT_MAX];
} EGG_CHART_T;

void egg_chart_init(EGG_CHART_T *obj, short x, short y, short w, short h, short r);
void egg_draw_chart(EGG_CHART_T *obj);
void egg_btn_onoff(EGG_SWITCH_T *obj, u8 onoff);
void egg_chart_add_val(EGG_CHART_T *obj,short val);

//-----------------------extern fun----------------------//
void egg_ui_init(void);
void egg_ui_run(void);
void egg_ui_init(void);
void egg_tick_cb(void);

extern EGG_BASE_T egg_base;
#endif
