typedef struct
{
    unsigned char count;
    int blocked_task_id;
} Semaphore_t;
void kira_semaphore_init(Semaphore_t *semaphore);
void kira_semaphore_wait(Semaphore_t *semaphore);
void kira_semaphore_signal(Semaphore_t *semaphore);