#ifndef STRUCT_H__
#define STRUCT_H__

#include <stdbool.h>
#include <stddef.h>

#define STACK_DEPTH 50001
#define MAX_FREED_NUM 1000


typedef struct
{
    bool is_operator;
    union 
    {
        double operand;
        char operator;
    } value;
} token_t;

typedef struct list_node_t list_node_t;

struct list_node_t
{
    token_t value;
    list_node_t *next_node;
};

typedef struct
{
    token_t tokens[STACK_DEPTH];
    size_t depth;
} array_stack_t;

typedef struct
{
    list_node_t *head;
    size_t depth;
} list_stack_t;

typedef struct
{
    void *stack;
    bool (*push)(void *stack, token_t value);
    bool (*pop)(void *stack, token_t *value);
    void (*free)(void *stack);
} stack_t;


typedef struct
{
    size_t len;
    list_node_t *freed_nodes[MAX_FREED_NUM];
} freed_nodes_array_t;

typedef struct
{
    void *stack;
    bool (*push)(void *stack, token_t value);
    bool (*pop_mem)(void *stack, token_t *value, freed_nodes_array_t *freed_memory);
    bool (*print)(void *stack);
    freed_nodes_array_t *freed_memory;
    void (*print_freed)(freed_nodes_array_t *freed_memory);
    void (*free)(void *stack, freed_nodes_array_t *freed_memory);
} debug_stack_t;


#endif // #ifndef STRUCT_H__
