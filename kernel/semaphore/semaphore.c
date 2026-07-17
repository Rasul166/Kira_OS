#include<semaphore.h>
#include<config.h>
#include<task.h>
void kira_semaphore_init(Semaphore_t *semaphore)
{
    semaphore->count = 0;
    semaphore->blocked_task_id = -1;
}

void kira_semaphore_wait(Semaphore_t *semaphore)
{
    __disable_irq();
    if (semaphore->count == 1)
    {
        semaphore->count = 0;
        __enable_irq();
    }
    else
    {
        semaphore->blocked_task_id = current_task;
        Task_table[current_task].state = TASK_BLOCKED;
        kira_scheduler();
        __enable_irq();
    }
}

void kira_semaphore_signal(Semaphore_t *semaphore)
{
    __disable_irq();
    if (semaphore->blocked_task_id != -1)
    {
        Task_table[semaphore->blocked_task_id].state = TASK_READY;
        semaphore->blocked_task_id = -1;
        kira_scheduler();
    }
    else
    {
    }
    __enable_irq();
}