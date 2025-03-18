#ifndef STRUCT_H__
#define STRUCT_H__

#include <stdbool.h>
#include <stddef.h>

#define QUEUE_DEPTH 2000


typedef union
{
    double time;
    struct list_node_t *ptr;
} value_t;

typedef struct list_node_t
{
    value_t value;
    struct list_node_t *next_node;
} list_node_t;

typedef struct
{
    value_t values[QUEUE_DEPTH];
    int in;
    int out;
    int depth;
} array_queue_t;

typedef struct
{
    list_node_t *head;
    list_node_t *tail;
    int depth;
} list_queue_t;

typedef struct
{
    void *queue;
    bool (*push)(void *queue, value_t value);
    bool (*pop)(void *queue, value_t *value);
    void (*free)(void *queue);
    int (*get_len)(void *queue);
} abstract_queue_t;

typedef struct
{
    void *queue;
    bool (*push)(void *queue, value_t value);
    bool (*pop_mem)(void *queue, value_t *value, list_queue_t *freed_memory);
    bool (*print)(void *queue);
    list_queue_t *freed_memory;
    void (*print_freed)(list_queue_t *freed_memory);
    void (*free)(void *queue, list_queue_t *freed_memory);
} debug_queue_t;


#endif // #ifndef STRUCT_H__
