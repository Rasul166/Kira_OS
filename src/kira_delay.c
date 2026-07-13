#include <kira_delay.h>
#include <kira_task.h>
#include <kira_uart.h>
volatile unsigned int ticks = 0;

void delayms(int no_of_ms)
{
    unsigned int start = ticks;
    while (ticks - start < no_of_ms)
    {
    }
}
void SysTick_Handler(void)
{

    ticks++;
    for (int i = 0; i <task_count ; i++)
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
    kira_scheduler();
}
void kira_systick_init(void)
{

    loadreg = 71999;
    currreg = 0;
    ctrlreg |= (7);
}