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
void kira_task_create(void (*task_function)(void),unsigned int priority) {

    // Prevent array overflow if we try to create too many tasks
    if (task_count >= MAX_TASKS) {
        return; 
    }
    Task_table[task_count].state=TASK_READY;
    Task_table[task_count].sleep_ticks=0;
    Task_table[task_count].priority=priority;

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
  
    unsigned int highest_priority=0;
    int next_task_index=(task_count-1);
	  int i;
    for(i=0;i<task_count;i++)
    {
        if((Task_table[i].state==TASK_READY)&&(Task_table[i].priority>highest_priority))
        {
            highest_priority=Task_table[i].priority;
            next_task_index=i;
        }
    }
    next_task_pointer=&Task_table[next_task_index];
    current_task=next_task_index;
    scb_icsr|=(1<<28);
}
void kira_os_start(void){
	
	    
        kira_task_create(kira_idle_task,1);
	
			    __asm volatile ("svc 0");
	

}
void kira_task_sleep(unsigned int ms){

    Task_table[current_task].state=TASK_SLEEPING;
    Task_table[current_task].sleep_ticks=ms;
        kira_scheduler();

}
void kira_idle_task(void){
    while(1){
			
        __asm volatile("WFI");
      
    }
}
void kira_mutex_init(Mutex_t *mutex){
	
	mutex->is_locked=0;
 mutex->blocked_task_id=-1;
	mutex->owner_task_id=-1;
} 
void kira_mutex_take(Mutex_t *mutex){
__disable_irq();
	if(mutex->is_locked==0)
    {
		mutex->is_locked=1;
	mutex->owner_task_id=current_task;

	}
    else
    {
	mutex->blocked_task_id=current_task;
	Task_table[current_task].state=TASK_BLOCKED;
    }	
	kira_scheduler();
    __enable_irq();
    
} 	
        
	

void kira_mutex_give(Mutex_t *mutex){
	__disable_irq();
if (mutex->owner_task_id == current_task){
    if(mutex->blocked_task_id!=-1)
    {  
        Task_table[mutex->blocked_task_id].state=TASK_READY;
        mutex->owner_task_id=mutex->blocked_task_id;
        mutex->blocked_task_id=-1;
    }    
    else
     {
        mutex->is_locked=0;
        mutex->owner_task_id=-1;
     }
    
	}
    
    kira_scheduler();
    __enable_irq();
}

void kira_semaphore_init(Semaphore_t *semaphore)
{
    semaphore->count=0;
    semaphore->blocked_task_id=-1;
}

void kira_semaphore_wait(Semaphore_t *semaphore)
{
    __disable_irq();
    if(semaphore->count==1)
    {
        semaphore->count=0;
        __enable_irq();
    }
    else
    {
        semaphore->blocked_task_id=current_task;
        Task_table[current_task].state=TASK_BLOCKED;
        kira_scheduler();
        __enable_irq();
    }
}

void kira_semaphore_signal(Semaphore_t *semaphore)
{
    __disable_irq();
    if(semaphore->blocked_task_id!=-1)
    {
        Task_table[semaphore->blocked_task_id].state=TASK_READY;
        semaphore->blocked_task_id=-1;
        kira_scheduler();
    }
    else
    {
        semaphore->count=1;
    }
    __enable_irq();
}
