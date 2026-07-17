#define KIRA_PRODUCER_CONSUMER_BUFFER(TYPE, NAME, SIZE)                                               \
    KIRA_BUFFER_DEFINE(TYPE, NAME, SIZE)                                                              \
    typedef struct                                                                                    \
    {                                                                                                 \
        NAME buffer_t;                                                                                \
        List blocked_producer_tasks;                                                                  \
        List blocked_consumer_tasks;                                                                  \
    } Kira_Queue_t;                                                                                   \
                                                                                                      \
    void kira_queue_send(TYPE cstm_data, Kira_Queue_t *kira_queue)                                    \
    {                                                                                                 \
        __disable_irq();                                                                              \
        while (NAME##_isFull(&kira_queue->cstm_buffer))                                               \
        {                                                                                             \
            list_push_back(&kira_queue->blocked_producer_tasks, &Task_table[current_task].task_node); \
            Task_table[current_task].state = TASK_BLOCKED;                                            \
            __enable_irq();                                                                           \
            kira_scheduler();                                                                         \
            __disable_irq();                                                                          \
        }                                                                                             \
                                                                                                      \
        NAME##_push(kira_queue, cstm_data);                                                           \
        if (!list_is_empty(&kira_queue->blocked_consumer_tasks))                                      \
        {                                                                                             \
            int highest_consumer_blocked_id;                                                          \
            ListNode *temp_node;                                                                      \
            int priority = 0;                                                                         \
            while (!list_is_empty(&kira_queue))                                                       \
            {                                                                                         \
                temp_node = list_pop_back(&kira_queue);                                               \
                if (CONTAINER_OF(temp_node, TCB_t, task_node)->current_priority > priority)           \
                {                                                                                     \
                    priority = CONTAINER_OF(temp_node, TCB_t, task_node)->current_priority;           \
                    highest_consumer_blocked_id = CONTAINER_OF(temp_node, TCB_t, task_node)->Task_Id; \
                }                                                                                     \
            }                                                                                         \
            Task_table[highest_consumer_blocked_id].state = TASK_READY;                               \
        }                                                                                             \
        __enable_irq();                                                                               \
    }                                                                                                 \
                                                                                                      \
    TYPE kira_queue_receive(Kira_Queue_t *kira_queue)                                                 \
    {                                                                                                 \
        __disable_irq();                                                                              \
                                                                                                      \
        while (NAME##_isEmpty(kira_queue))                                                            \
        {                                                                                             \
            list_push_back(&kira_queue->blocked_consumer_tasks, &Task_table[current_task].task_node); \
                                                                                                      \
            Task_table[current_task].state = TASK_BLOCKED;                                            \
            __enable_irq();                                                                           \
                                                                                                      \
            kira_scheduler();                                                                         \
            __disable_irq();                                                                          \
        }                                                                                             \
                                                                                                      \
        TYPE *cstm_data;                                                                              \
        cstm_data = NAME##_pop();                                                                     \
        if (!list_is_empty(&kira_queue->blocked_producer_tasks))                                      \
        {                                                                                             \
            int highest_producer_blocked_id;                                                          \
            ListNode *temp_node;                                                                      \
            int priority = 0;                                                                         \
            while (!list_is_empty(&kira_queue))                                                       \
            {                                                                                         \
                temp_node = list_pop_back(&kira_queue);                                               \
                if (CONTAINER_OF(temp_node, TCB_t, task_node)->current_priority > priority)           \
                {                                                                                     \
                    priority = CONTAINER_OF(temp_node, TCB_t, task_node)->current_priority;           \
                    highest_producer_blocked_id = CONTAINER_OF(temp_node, TCB_t, task_node)->Task_Id; \
                }                                                                                     \
            }                                                                                         \
            Task_table[highest_producer_blocked_id].state = TASK_READY;                               \
        }                                                                                             \
        return *cstm_data;                                                                            \
        __enable_irq();                                                                               \
    }
