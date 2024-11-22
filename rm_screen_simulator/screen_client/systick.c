#include "systick.h"
#include "time.h"
#include "Windows.h"
#include "stdio.h"

static uint64_t tick_counter;
static struct timespec time_base;

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


uint8_t ms_timer_create(HANDLE *timer_thread, irq_cb cb, uint32_t delay_ms) {
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