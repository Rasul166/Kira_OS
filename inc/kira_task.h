#include<stdint.h>
void kira_schedular(void);
typedef struct {
    uint32_t *sp; // The Stack Pointer
} TCB_t;
#define MAX_TASKS 3
#define STACK_SIZE 100
#define scb_shpr3 *((volatile unsigned int *)0xE000ED20)
#define scb_icsr *((volatile unsigned int *)0xE000ED04)
extern uint32_t Task_Stack[MAX_TASKS][STACK_SIZE]; // Physical RAM for the tasks
extern TCB_t Task_table[MAX_TASKS];                // The OS Task Table
extern volatile int current_task ;
extern volatile int task_count;

void kira_task_create(void (*task_function)(void)); 
=======
void kira_task_create(void (*task_function)(void)); 
>>>>>>> b8c01af80fd9b545a47bcc415b4cb1c37c41460d
