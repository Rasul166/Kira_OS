#include<kira_task.h>
#include<kira_uart.h>
volatile int current_task=0;
volatile int task_count=0;
void kira_task_create(void (*task_function)(void)) {
    // Prevent array overflow if we try to create too many tasks
    if (current_task >= MAX_TASKS) {
        return; 
    }

    // 1. Set the xPSR register to Thumb Mode (Index 99)
    Task_Stack[current_task][STACK_SIZE - 1] = 0x01000000;
    
    // 2. Set the Program Counter (PC) to the task's Flash memory address (Index 98)
    Task_Stack[current_task][STACK_SIZE - 2] = (uint32_t)task_function;
    
    // 3. Save the final Stack Pointer into our OS Task Table.
    // The top of the array is 100, and we pushed 16 fake registers.
    Task_table[current_task].sp = &Task_Stack[current_task][STACK_SIZE - 16];
    
    // 4. Move to the next slot in the OS table for the next task
    current_task++;
}
void kira_schedular(void){

    kira_print_string("Scheduler Fired!\n");
    current_task++;
    if(current_task>=task_count)task_count=0;
    scb_icsr|=(1<<28);
}