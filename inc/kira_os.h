void delayms(int no_of_ms);

//List
typedef struct ListNode;
typedef struct List;
void list_init(List *list);
bool list_is_empty(List *list);
void list_push_front(List *list,ListNode *node);
void list_push_back(List *list,ListNode *node);
void list_remove(List *list,ListNode *node);
ListNode *list_pop_front(List *list);
ListNode *list_pop_back(List *list);
//from task.c
void kira_task_create(void (*task_function)(void), unsigned int priority);
void kira_task_sleep(unsigned int ms);
typedef struct Mutex_t;
void kira_mutex_init(Mutex_t *mutex);
void kira_mutex_take(Mutex_t *mutex);
void kira_mutex_give(Mutex_t *mutex);
typedef struct Semaphore_t;
typedef struct Custom_data;
typedef struct Kira_Queue_t;
void kira_semaphore_init(Semaphore_t *semaphore);
void kira_semaphore_wait(Semaphore_t *semaphore);
void kira_semaphore_signal(Semaphore_t *semaphore);

void kira_queue_send(Custom_data cstm_data, Kira_Queue_t *kira_queue_t);
Custom_data kira_queue_receive(Kira_Queue_t *kira_queue_t);

//uart.h
void kira_print_time_and_text(char *text);
void kira_uart_init(void);
void kira_print_string(char *str);
void kira_print_char(char c);

void ring_buffer_push(char data);
char ring_buffer_pop(void);
void kira_print_int(int nums);