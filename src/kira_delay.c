#include <kira_delay.h>
#include <kira_task.h>
#include <kira_uart.h>
#include <kira_timer.h>
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
    

for(int i=0;i<timer_count;i++)
{
    for(int i=0;i<timer_count;i++)
    {
        if(LOST[i].state==1&&LOST[i].Time_remaining>0)
        {
            LOST[i].Time_remaining--;
            if(LOST[i].Time_remaining==0)
            {
            exp_timers[i]=1;
            if(LOST[i].mode=='r')
            LOST[i].Time_remaining=LOST[i].period;
            else
            LOST[i].state=0;
            Task_table[daemon_task_id].state=TASK_READY;
            }
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