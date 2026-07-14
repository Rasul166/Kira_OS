#include <kira_timer.h>
#include <kira_ds.h>


volatile int timer_count=0;
volatile int daemon_task_id=-1;
int exp_timers[15];
 Kira_timer_t LOST[15];
command_queue cmd_q;


void kira_timer_command_send(int timerid,int op)
{
    __disable_irq();
     kira_timer_command ab;
    ab.timer_id=timerid;ab.operation=op;
    command_queue_push(&cmd_q,ab);
    __enable_irq();
}   


kira_timer_command  kira_timer_command_receive(void)
{
    __disable_irq();
    kira_timer_command command_receive;
    command_queue_pop(&cmd_q,&command_receive);
    __enable_irq();
    return command_receive;
}


   void kira_timer_create(int Timer_id,int period,char mode,void (*fu_ptr)(void))
   {
    __disable_irq();
    if(timer_count>15)
    {
        __enable_irq();
        return;
    }
    LOST[timer_count].Timer_id=Timer_id;
    LOST[timer_count].period=period;
    LOST[timer_count].Time_remaining=period;
    LOST[timer_count].mode=mode;
    LOST[timer_count].func_ptr=fu_ptr;
    LOST[timer_count].state=0;
    timer_count++;
    __enable_irq();
   }
