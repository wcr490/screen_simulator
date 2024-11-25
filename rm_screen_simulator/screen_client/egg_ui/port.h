#ifndef __PORT_H__
#define __PORT_H__

#include "../port_u8g2.h"
#include "../systick.h"

uint32_t SysTick_GetTick(void);
HANDLE egg_tick_init(void);

#endif