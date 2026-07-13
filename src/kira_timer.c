#include <kira_timer.h>

volatile int timer_count=0;
volatile int daemon_task_id=-1;


void kira_timer_command_send(int timerid,int op)
{
    disable__irq();
     kira_timer_command ab;
    ab.timer_id=timerid;ab.operation=op;
    command_queue_push(&cmd_q,ab);
    enable_irq();
}   


void  kira_timer_command_receive()
{
    disable__irq();
    command_queue_pop(&cmd_q,&abc);
    enable__irq();

}

