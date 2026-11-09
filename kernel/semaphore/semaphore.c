#include <semaphore.h>
#include <config.h>
#include <task.h>
#include <common.h>
void kira_semaphore_init(Semaphore_t *semaphore)
{
    semaphore->interrupt_fired = 0;
}

void kira_semaphore_wait(Semaphore_t *semaphore)
{
    __disable_irq();
    if (semaphore->interrupt_fired == 1)
    {
        semaphore->interrupt_fired = 0;
        __enable_irq();
    }
    else
    {
        list_push_front(&(semaphore->waiting_list), &(Task_table[current_task].task_node));
        Task_table[current_task].state = TASK_BLOCKED;
        kira_scheduler();
        __enable_irq();
    }
}

void kira_semaphore_signal(Semaphore_t *semaphore)
{
    __disable_irq();
    semaphore->interrupt_fired = 1;
    if (!list_is_empty(semaphore->waiting_list))
    {
        int priority = 0;
        int highest_priority_waiting_task_id;
        while (!list_is_empty(semaphore->waiting_list))
        {
            ListNode *temp_node = list_pop_back(&semaphore->waiting_list);
            if (CONTAINER_OF(temp_node, TCB_t, task_node)->current_priority > priority)
            {
                priority = CONTAINER_OF(temp_node, TCB_t, task_node)->current_priority;
                highest_priority_waiting_task_id = CONTAINER_OF(temp_node, TCB_t, task_node)->Task_Id;
            }
        }
        Task_table[highest_priority_waiting_task_id].state = TASK_READY;

        kira_scheduler();
    }
   
    __enable_irq();
}