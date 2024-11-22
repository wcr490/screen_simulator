#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "stdint.h"
#include "time.h"
#include <windows.h>

#define TIME_UTC 1

uint8_t systick_init();
uint64_t systick_get_tick_ms();
uint64_t systick_get_tick_us();
int timespec_get(struct timespec *ts, int base) {
    if (base != TIME_UTC) {
        return 0; // 不支持的时间基准
    }

    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    uint64_t time_ns = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;

    time_ns = (time_ns - 116444736000000000ULL) * 100;

    ts->tv_sec = time_ns / 1000000000ULL;
    ts->tv_nsec = time_ns % 1000000000ULL;

    return base; 
}

typedef void (*irq_cb)(void);

uint8_t ms_timer_create(HANDLE *timer_thread, irq_cb* cb, uint32_t delay_ms);
uint8_t ms_timer_delete(HANDLE *timer_thread);
DWORD WINAPI TimerThread(LPVOID lpParam, irq_cb cb) {
    uint32_t delay_ms = *(uint32_t *)lpParam;
    while (1) {
        Sleep(delay_ms);
        cb();
    }
    return 0;
}

#endif