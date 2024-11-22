#include "../systick.h"
#include "egg_ui.h"

uint32_t SysTick_GetTick(void) {
    return systick_get_tick_ms();
}
HANDLE egg_tick_init(void) {
    HANDLE timer;
    if (ms_timer_create(&timer, (irq_cb *)egg_tick_cb, 1))
        return NULL;
    return timer;
}