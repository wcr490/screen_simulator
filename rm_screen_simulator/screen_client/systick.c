#include "systick.h"
#include "time.h"
#include "Windows.h"
#include "stdio.h"

uint64_t tick_counter;
struct timespec time_base;

uint8_t systick_init() {
    if (!timespec_get(&time_base, 1)) {
        return 1;
    }
    tick_counter = 0;
    return 0;
}
uint64_t systick_get_tick_ms() {
    struct timespec time_now;
    if (!timespec_get(&time_now, 1)) {
        return tick_counter;
    }
    uint64_t cur_counter = (time_now.tv_sec - time_base.tv_sec) * 1000 + (time_now.tv_nsec - time_base.tv_nsec) / 1000000;
    tick_counter = cur_counter;
    return cur_counter;
}

uint64_t systick_get_tick_us() {
    struct timespec time_now;
    if (!timespec_get(&time_now, 1)) {
        return tick_counter;
    }
    uint64_t cur_counter = (time_now.tv_sec - time_base.tv_sec) * 1000000 + (time_now.tv_nsec - time_base.tv_nsec) / 1000;
    tick_counter = cur_counter;
    return cur_counter;
}


uint8_t ms_timer_create(HANDLE *timer_thread, uint32_t delay_ms) {
    HANDLE thread = CreateThread(NULL, 0, TimerThread, &delay_ms, 0, NULL); 
    if (thread == NULL) {
        return 1;
    }
    *timer_thread = thread;
    return 0;
}
uint8_t ms_timer_delete(HANDLE *timer) {
    if (timer) {
        TerminateThread(timer, 0);
        CloseHandle(timer);
        free(timer);
    }
}

int test(void) {
    if (systick_init()) {
        printf("failed");
        return 1;
    }
    while (systick_get_tick_ms() < 1000) {
    }
    printf("end: %d\n", tick_counter);
    return 0;
}

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

DWORD WINAPI TimerThread(LPVOID lpParam) {
    uint32_t delay_ms = *(uint32_t *)lpParam;
    while (1) {
        Sleep(delay_ms);
        irq_callback();
    }
    return 0;
}

