#include<config.h>
#include<stdint.h>
#include<kira_ds.h>
typedef struct
{
    uint32_t *sp;
    unsigned char state;
    unsigned int sleep_ticks;
    int current_priority;
    int base_priority;
    int Task_Id;
    ListNode task_node;
    List owned_mutexes;
} TCB_t;

#define TASK_READY 0
#define TASK_SLEEPING 1
#define TASK_BLOCKED 2


extern uint32_t Task_Stack[MAX_TASKS][STACK_SIZE]; // Physical RAM for the tasks
extern TCB_t Task_table[MAX_TASKS];                // The OS Task Table
extern volatile int *current_task_pointer;
extern volatile int *next_task_pointer;
extern volatile int current_task;
extern volatile int task_count;
int kira_task_create(void (*task_function)(void), unsigned int priority);
void kira_task_sleep(unsigned int ms);
void kira_idle_task(void);
