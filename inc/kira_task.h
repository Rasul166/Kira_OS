#include<stdint.h>
typedef struct {
    uint32_t *sp; // The Stack Pointer
} TCB_t;
#define MAX_TASKS 3
#define STACK_SIZE 100

uint32_t Task_Stack[MAX_TASKS][STACK_SIZE]; // Physical RAM for the tasks
TCB_t Task_table[MAX_TASKS];                // The OS Task Table
int current_task = 0;
void kira_task_create(void (*task_function)(void)); 