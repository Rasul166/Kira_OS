#include<mutex.h>
#include<config.h>
#include<task.h>
void kira_mutex_init(Mutex_t *mutex)
{

    mutex->is_locked = 0;
    mutex->no_of_blocked_tasks = 0;
    mutex->owner_task_id = -1;
    mutex->highest_priority = -1;
    for (int i = 0; i < MAX_TASKS; i++)
    {
        mutex->arr_bt[i] = -1;
    }
}


void kira_mutex_take(Mutex_t *mutex)
{
    __disable_irq();
    if (mutex->is_locked == 0)
    {
        mutex->is_locked = 1;
        mutex->owner_task_id = current_task;
        list_push_back(&Task_table[current_task].owned_mutexes, &mutex->owner_node);
    }
    else

    {
        if (mutex->highest_priority < Task_table[current_task].current_priority)
            mutex->highest_priority = Task_table[current_task].current_priority;
        if (Task_table[mutex->owner_task_id].current_priority < Task_table[current_task].current_priority)
            Task_table[mutex->owner_task_id].current_priority = Task_table[current_task].current_priority; // boosting
        mutex->no_of_blocked_tasks++;
        Task_table[current_task].state = TASK_BLOCKED;
        mutex->arr_bt[current_task] = Task_table[current_task].current_priority;
    }
    __enable_irq();
    kira_scheduler();
}


void kira_mutex_give(Mutex_t *mutex)
{
    __disable_irq();
    if (mutex->owner_task_id == current_task)
    {
        if (mutex->no_of_blocked_tasks != 0)
        {
            int max = -1;
            int c;
            for (int j = 0; j < 10; j++)
            {
                if (mutex->arr_bt[j] > max)
                {
                    max = mutex->arr_bt[j];
                    c = j;
                }
            }
            Task_table[c].state = TASK_READY;
            mutex->owner_task_id = c;
            mutex->arr_bt[c] = 0;
            mutex->no_of_blocked_tasks--;
            list_remove(&Task_table[current_task].owned_mutexes, &mutex->owner_node);
            int priority = Task_table[current_task].base_priority;

            ListNode *node = Task_table[current_task].owned_mutexes.head;
            while (node)
            {
                Mutex_t *mutex = CONTAINER_OF(node, Mutex_t, owner_node);
                if (priority < mutex->highest_priority)
                {
                    priority = mutex->highest_priority;
                }
                node = node->next;
            }
            Task_table[current_task].current_priority = priority;
        }
        else
        {
            mutex->is_locked = 0;
            mutex->owner_task_id = -1;
        }
    }
    __enable_irq();
    kira_scheduler();
}
