#include<kira_ds.h>
typedef struct
{
    unsigned char is_locked;
    int owner_task_id;
    int no_of_blocked_tasks;
    int arr_bt[10];
    int highest_priority;
    ListNode owner_node;

} Mutex_t;

void kira_mutex_init(Mutex_t *mutex);
void kira_mutex_take(Mutex_t *mutex);
void kira_mutex_give(Mutex_t *mutex);