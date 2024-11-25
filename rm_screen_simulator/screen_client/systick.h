#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "stdint.h"
#include "time.h"
#include <windows.h>

#define TIME_UTC 1

uint8_t systick_init();
uint64_t systick_get_tick_ms();
uint64_t systick_get_tick_us();
int timespec_get(struct timespec *ts, int base);

typedef void (*irq_cb)(void);

extern irq_cb irq_callback;

uint8_t ms_timer_create(HANDLE *timer_thread, uint32_t delay_ms);
uint8_t ms_timer_delete(HANDLE *timer_thread);
DWORD WINAPI TimerThread(LPVOID lpParam);

#endif