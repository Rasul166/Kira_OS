#include<stdint.h>
typedef struct {
    uint32_t *sp;
    unsigned char state;
    unsigned int sleep_ticks; // The Stack Pointer
} TCB_t;
typedef struct{
	unsigned char is_locked;
	int owner_task_id;
	int blocked_task_id;

} Mutex_t;
#define TASK_READY 0
#define TASK_SLEEPING 1
#define TASK_BLOCKED 2
#define MAX_TASKS 3
#define STACK_SIZE 100
#define scb_shpr3 *((volatile unsigned int *)0xE000ED20)
#define scb_icsr *((volatile unsigned int *)0xE000ED04)
extern uint32_t Task_Stack[MAX_TASKS][STACK_SIZE]; // Physical RAM for the tasks
extern TCB_t Task_table[MAX_TASKS];                // The OS Task Table
extern volatile int *current_task_pointer ;
extern volatile int *next_task_pointer;
extern volatile int current_task;
extern volatile int task_count;
void kira_scheduler(void);
void kira_task_create(void (*task_function)(void)); 
void kira_task_sleep(unsigned int ms);
void kira_os_start(void);
void kira_idle_task(void);
void kira_mutex_init(Mutex_t *mutex);
void kira_mutex_take(Mutex_t *mutex);	
void kira_mutex_give(Mutex_t *mutex);
void __disable_irq();
void __enable_irq();