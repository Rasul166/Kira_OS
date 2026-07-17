#include<task.h>
int kira_task_create(void (*task_function)(void), unsigned int priority)
{

    // Prevent array overflow if we try to create too many tasks
    if (task_count >= MAX_TASKS)
    {
        return -1;
    }
    Task_table[task_count].state = TASK_READY;
    Task_table[task_count].sleep_ticks = 0;
    Task_table[task_count].base_priority = priority;
    Task_table[task_count].current_priority = priority;
    Task_table[task_count].Task_Id=task_count;
    // 1. Set the xPSR register to Thumb Mode (Index 99)
    Task_Stack[task_count][STACK_SIZE - 1] = 0x01000000;

    // 2. Set the Program Counter (PC) to the task's Flash memory address (Index 98)
    Task_Stack[task_count][STACK_SIZE - 2] = (uint32_t)task_function;

    // 3. Save the final Stack Pointer into our OS Task Table.
    // The top of the array is 100, and we pushed 16 fake registers.
    Task_table[task_count].sp = &Task_Stack[task_count][STACK_SIZE - 16];
    Task_Stack[task_count][0] = 0xDEADBEEF;
    // 4. Move to the next slot in the OS table for the next task
    task_count++;
    return task_count;
}

void kira_task_sleep(unsigned int ms)
{

    Task_table[current_task].state = TASK_SLEEPING;
    Task_table[current_task].sleep_ticks = ms;
    kira_scheduler();
}
void kira_idle_task(void)
{
    while (1)
    {

        __asm volatile("WFI");
    }
}
