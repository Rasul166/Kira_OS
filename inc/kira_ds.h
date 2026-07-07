#ifndef kira_ds
#define kira_ds
#include <stdint.h>
#include <stdbool.h>
#include<stdlib.h>
/*-----------------------------------------------------------
 * KIRA_BUFFER_DEFINE(TYPE, NAME, SIZE)
 *
 * Example:
 *      KIRA_BUFFER_DEFINE(char, UART_Buffer, 64)
 *      KIRA_BUFFER_DEFINE(CustomData, Sensor_Buffer, 32)
 *----------------------------------------------------------*/

#define KIRA_BUFFER_DEFINE(TYPE, NAME, SIZE)             \
    typedef struct                                       \
    {                                                    \
        TYPE buffer[SIZE];                               \
        uint16_t head;                                   \
        uint16_t tail;                                   \
        uint16_t count;                                  \
    } NAME;                                              \
    static inline void NAME##_init(NAME *cb)             \
    {                                                    \
        cb->head = 0;                                    \
        cb->tail = 0;                                    \
        cb->count = 0;                                   \
    }                                                    \
    static inline bool NAME##_isEmpty(NAME *cb)          \
    {                                                    \
        return (cb->count == 0);                         \
    }                                                    \
    static inline bool NAME##_isFull(NAME *cb)           \
    {                                                    \
        return (cb->count == SIZE);                      \
    }                                                    \
    static inline uint16_t NAME##_size(NAME *cb)         \
    {                                                    \
        return cb->count;                                \
    }                                                    \
    static inline void NAME##_clear(NAME *cb)            \
    {                                                    \
        cb->head = 0;                                    \
        cb->tail = 0;                                    \
        cb->count = 0;                                   \
    }                                                    \
    static inline bool NAME##_push(NAME *cb, TYPE value) \
    {                                                    \
        if (NAME##_isFull(cb))                           \
            return false;                                \
                                                         \
        cb->buffer[cb->head] = value;                    \
        cb->head = (cb->head + 1) % SIZE;                \
        cb->count++;                                     \
                                                         \
        return true;                                     \
    }                                                    \
    static inline bool NAME##_pop(NAME *cb, TYPE *value) \
    {                                                    \
        if (NAME##_isEmpty(cb))                          \
            return false;                                \
                                                         \
        *value = cb->buffer[cb->tail];                   \
        cb->tail = (cb->tail + 1) % SIZE;                \
        cb->count--;                                     \
                                                         \
        return true;                                     \
    }
typedef struct ListNode{

    struct ListNode *next;
    struct ListNode *prev;
}ListNode;
typedef struct{
    ListNode *head;
    ListNode *tail;
    unsigned int size;
}List;

void list_init(List *list);
bool list_is_empty(List *list);
void list_push_front(List *list,ListNode *node);
void list_push_back(List *list,ListNode *node);
void list_remove(List *list,ListNode *node);
ListNode *list_pop_front(List *list);
ListNode *list_pop_back(List *list);

#define OFFSET_OF(type, member) \
    ((size_t)&(((type *)0)->member))

#define CONTAINER_OF(ptr, type, member) \
    ((type *)((char *)(ptr) - OFFSET_OF(type, member)))
#endif