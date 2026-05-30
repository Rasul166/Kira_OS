#include<kira_task.h>
#include<kira_uart.h>
volatile int *current_task_pointer;
volatile int *next_task_pointer;
volatile int task_count=0;
current_task=&Task_table[0];
 uint32_t Task_Stack[MAX_TASKS][STACK_SIZE]; // Physical RAM for the tasks
 TCB_t Task_table[MAX_TASKS]; 
void kira_task_create(void (*task_function)(void)) {
    // Prevent array overflow if we try to create too many tasks
    if (task_count >= MAX_TASKS) {
        return; 
    }

    // 1. Set the xPSR register to Thumb Mode (Index 99)
    Task_Stack[task_count][STACK_SIZE - 1] = 0x01000000;
    
    // 2. Set the Program Counter (PC) to the task's Flash memory address (Index 98)
    Task_Stack[task_count][STACK_SIZE - 2] = (uint32_t)task_function;
    
    // 3. Save the final Stack Pointer into our OS Task Table.
    // The top of the array is 100, and we pushed 16 fake registers.
    Task_table[task_count].sp = &Task_Stack[task_count][STACK_SIZE - 16];
    
    // 4. Move to the next slot in the OS table for the next task
    task_count++;
}
void kira_scheduler(void){

    kira_print_string("Scheduler Fired!\n");
    current_task++;

    if(current_task>=task_count)current_task=0;
    current_task_pointer=&Task_table[current_task];
    scb_icsr|=(1<<28);
}
void kira_os_start(void){
    _asm("SVC #0");
}
