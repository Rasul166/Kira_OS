#include <kira_task.h>
#include <kira_uart.h>
#include <stdbool.h>
#include <kira_ds.h>
#define TASK_READY 0
#define TASK_SLEEPING 1
#define TASK_BLOCKED 2
volatile int *current_task_pointer = &Task_table[0];
volatile int *next_task_pointer = &Task_table[1];
volatile int current_task = 0;
volatile int task_count = 0;

uint32_t Task_Stack[MAX_TASKS][STACK_SIZE]; // Physical RAM for the tasks
TCB_t Task_table[MAX_TASKS];

void kira_task_create(void (*task_function)(void), unsigned int priority)
{

    // Prevent array overflow if we try to create too many tasks
    if (task_count >= MAX_TASKS)
    {
        return;
    }
    Task_table[task_count].state = TASK_READY;
    Task_table[task_count].sleep_ticks = 0;
    Task_table[task_count].base_priority = priority;
    Task_table[task_count].current_priority = priority;

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
}
/* Kernel Invariant:
 * Idle task is created in OS_Start() after all user tasks.
 * No task creation is allowed once the scheduler starts.
 * Therefore, the idle task always resides at task_count - 1.
 */
void kira_scheduler(void)
{

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
    if (Task_Stack[current_task][0] != 0xDEADBEEF)
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
    current_task = next_task_index;
    scb_icsr |= (1 << 28);
}
void kira_os_start(void)
{

    kira_task_create(kira_idle_task, 1);

    __asm volatile("svc 0");
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
void kira_mutex_init(Mutex_t *mutex)
{

    mutex->is_locked = 0;
    mutex->no_of_blocked_tasks = 0;
    mutex->owner_task_id = -1;
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
        if (Task_table[mutex->owner_task_id].current_priority < Task_table[current_task].current_priority)
            Task_table[mutex->owner_task_id].current_priority = Task_table[current_task].current_priority; // boosting
        mutex->no_of_blocked_tasks++;
        Task_table[current_task].state = TASK_BLOCKED;
        mutex->arr_bt[current_task] = Task_table[current_task].current_priority;
    }
    kira_scheduler();
    __enable_irq();
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
                if (priority < Task_table[mutex->owner_task_id].current_priority)
                {
                    priority = Task_table[mutex->owner_task_id].current_priority;
                }
            }
            Task_table[current_task].current_priority = priority;
        }
        else
        {
            mutex->is_locked = 0;
            mutex->owner_task_id = -1;
        }
    }

    kira_scheduler();
    __enable_irq();
}

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
        __enable_irq();
}
void kira_queue_send(Custom_data cstm_data, Kira_Queue_t *kira_queue)
{
    __disable_irq();
    bool flag = true;
    if (Custom_Data_Buffer_isFull(&kira_queue->cstm_buffer))
    {
        kira_queue->blocked_task_id = current_task;
        Task_table[current_task].state = TASK_BLOCKED;
        kira_scheduler();
        __enable_irq();
        flag = false;
    }
    if (!flag)
        __disable_irq();
    Custom_Data_Buffer_push(kira_queue, cstm_data);
    if (kira_queue->blocked_task_id != -1)
        Task_table[kira_queue->blocked_task_id].state = TASK_READY;
    __enable_irq();
}
Custom_data kira_queue_receive(Kira_Queue_t *kira_queue)
{
    __disable_irq();
    bool flag = true;
    if (Custom_Data_Buffer_isEmpty(kira_queue))
    {
        kira_queue->blocked_task_id = current_task;
        Task_table[current_task].state = TASK_BLOCKED;
        kira_scheduler();
        __enable_irq();
        flag = false;
    }

    Custom_data *cstm_data;
    Custom_Data_Buffer_pop(kira_queue, cstm_data);
    if (kira_queue->blocked_task_id != -1)
        Task_table[kira_queue->blocked_task_id].state = TASK_READY;
    __enable_irq();
    return *cstm_data;
}
kira_kernel_panic(char *reason)
{
    __disable_irq();
    while (true)
    {
    }
    __enable_irq();
}