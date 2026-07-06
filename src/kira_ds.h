#ifndef kira_ds
#define kira_ds
#include <stdint.h>
#include <stdbool.h>
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
#endif