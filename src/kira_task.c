 #include<kira_task.h>
#include<kira_uart.h>
#define TASK_READY 0
#define TASK_SLEEPING 1
#define TASK_BLOCKED 2
volatile int *current_task_pointer=&Task_table[0];
volatile int *next_task_pointer=&Task_table[1];
volatile int current_task=0;
volatile int task_count=0;

 uint32_t Task_Stack[MAX_TASKS][STACK_SIZE]; // Physical RAM for the tasks
 TCB_t Task_table[MAX_TASKS]; 
void kira_task_create(void (*task_function)(void)) {
    // Prevent array overflow if we try to create too many tasks
    if (task_count >= MAX_TASKS) {
        return; 
    }
    Task_table[task_count].state=TASK_READY;
    Task_table[task_count].sleep_ticks=0;

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

    int temp=current_task;

    do{
         current_task=(current_task+1)%task_count-1;
    }while(temp!=current_task&&Task_table[current_task].state!=TASK_READY);
    if(Task_table[current_task].state==TASK_READY)
     {next_task_pointer=&Task_table[current_task];}
    else
    {next_task_pointer=&Task_table[task_count-1];}
    scb_icsr|=(1<<28);
}
void kira_os_start(void){
	    kira_print_string("os_s");
        kira_task_create(kira_idle_task);
			    __asm volatile ("svc 0");

}
void kira_task_sleep(unsigned int ms){
    Task_table[current_task].state=TASK_SLEEPING;
    Task_table[current_task].sleep_ticks=ms;
        scb_icsr|=(1<<28);

}
void kira_idle_task(void){
    while(1){
        __asm volatile("WFI");
        kira_print_string("i  ");
    }
}