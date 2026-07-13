#include<kira_ds.h>

typedef struct
{
    uint32_t *sp;
    unsigned char state;
    unsigned int sleep_ticks;
    int current_priority;
    int base_priority;
    List owned_mutexes;
} TCB_t;
typedef struct
{
    unsigned char is_locked;
    int owner_task_id;
    int no_of_blocked_tasks;
    int arr_bt[10];
    int highest_priority;
    ListNode owner_node;

} Mutex_t;
typedef struct
{
    unsigned char count;
    int blocked_task_id;
} Semaphore_t;
typedef struct
{
    int sensor_id;
    float temperature;
} Custom_data;
KIRA_BUFFER_DEFINE(Custom_data, Custom_Data_Buffer, 64);
typedef struct
{
    Custom_Data_Buffer cstm_buffer;
    int blocked_task_id;
} Kira_Queue_t;


#define TASK_READY 0
#define TASK_SLEEPING 1
#define TASK_BLOCKED 2
#define MAX_TASKS 10
#define STACK_SIZE 100
#define scb_shpr3 *((volatile unsigned int *)0xE000ED20)
#define scb_icsr *((volatile unsigned int *)0xE000ED04)
extern uint32_t Task_Stack[MAX_TASKS][STACK_SIZE]; // Physical RAM for the tasks
extern TCB_t Task_table[MAX_TASKS];                // The OS Task Table
extern volatile int *current_task_pointer;
extern volatile int *next_task_pointer;
extern volatile int current_task;
extern volatile int task_count;
void kira_scheduler(void);
int kira_task_create(void (*task_function)(void), unsigned int priority);
void kira_task_sleep(unsigned int ms);
void kira_os_start(void);
void kira_idle_task(void);
void kira_daemon_task(void);
void kira_mutex_init(Mutex_t *mutex);
void kira_mutex_take(Mutex_t *mutex);
void kira_mutex_give(Mutex_t *mutex);
void __disable_irq();
void __enable_irq();
void kira_semaphore_init(Semaphore_t *semaphore);
void kira_semaphore_wait(Semaphore_t *semaphore);
void kira_semaphore_signal(Semaphore_t *semaphore);
void kira_queue_send(Custom_data cstm_data, Kira_Queue_t *kira_queue_t);
Custom_data kira_queue_receive(Kira_Queue_t *kira_queue_t);