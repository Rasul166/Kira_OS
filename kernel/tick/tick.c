#include <config.h>
#include <timer.h>
#include<task.h>
#include<registers.h>
volatile unsigned int ticks = 0;
unsigned int reload_value = KIRA_CPU_CLOCK_HZ / KIRA_TICK_RATE_HZ;

void kira_systick_init(void)
{

    SYSTICK_LOAD = reload_value;
    SYSTICK_VAL = 0;
    SYSTICK_CTRL |= (7);
}

void SysTick_Handler(void)
{

    ticks++;
    for (int i = 0; i < task_count; i++)
    {
        if (Task_table[i].state == TASK_SLEEPING)
        {
            Task_table[i].sleep_ticks--;
            if (Task_table[i].sleep_ticks == 0)
            {
                Task_table[i].state = TASK_READY;
            }
        }
    }

    for (int i = 0; i < timer_count; i++)
    {
        if (LOST[i].state == 1 && LOST[i].Time_remaining > 0)
        {
            LOST[i].Time_remaining--;
            if (LOST[i].Time_remaining == 0)
            {
                exp_timers[i] = 1;
                if (LOST[i].mode == 'r')
                    LOST[i].Time_remaining = LOST[i].period;
                else
                    LOST[i].state = 0;
                Task_table[daemon_task_id].state = TASK_READY;
            }
        }
    }

    if (task_count > 0)
        kira_scheduler();
}
