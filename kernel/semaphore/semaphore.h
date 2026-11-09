typedef struct
{
    unsigned char interrupt_fired;
    List waiting_list;
} Semaphore_t;
void kira_semaphore_init(Semaphore_t *semaphore);
void kira_semaphore_wait(Semaphore_t *semaphore);
void kira_semaphore_signal(Semaphore_t *semaphore);