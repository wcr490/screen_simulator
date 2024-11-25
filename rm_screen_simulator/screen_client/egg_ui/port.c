#include "port.h"

void egg_tick_cb(void);
irq_cb irq_callback = egg_tick_cb;

uint32_t SysTick_GetTick(void)
{
    return systick_get_tick_ms();
}
HANDLE egg_tick_init(void)
{
    HANDLE timer;
    if (ms_timer_create(&timer, 1))
        return NULL;
    return timer;
}

