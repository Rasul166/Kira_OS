#include<task.h>
#include<registers.h>
/* Kernel Invariant:
 * Idle task is created in OS_Start() after all user tasks.
 * No task creation is allowed once the scheduler starts.
 * Therefore, the idle task always resides at task_count - 1.
 */
void kira_scheduler(void)
{
    int temp;
    int highest_priority = -1;
    int next_task_index = (task_count - 1);
    int i;
    for (i = 1; i < task_count; i++)
    {
        int index = (current_task + i) % (task_count - 1);
        if ((Task_table[index].state == TASK_READY) && (Task_table[index].current_priority > highest_priority))
        {
            highest_priority = Task_table[index].current_priority;
            next_task_index = index;
        }
    }
    next_task_pointer = &Task_table[next_task_index];
    if (Task_Stack[current_task][0] != 0xDEADBEEF && task_count != 0)
    {
        __disable_irq();
        kira_print_string("Task Number is : ");
        kira_print_int(current_task);
        kira_print_string("Overflowed");
        while (true)
        {
        }
        __enable_irq();
    }
    temp = current_task;
    current_task = next_task_index;
    if (temp != next_task_index)
        SCB_ICSR |= (1 << 28);
}