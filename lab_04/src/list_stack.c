#include <stdio.h>
#include <malloc.h>

#include "print.h"
#include "list_stack.h"

static token_t *see_elem_list(list_stack_t *stack, size_t ind)
{
    list_node_t *node = stack->head;
    for (size_t i = 0; i < ind; i++)
        node = node->next_node;
    return &node->value;
}

bool print_list(void *list_stack)
{
    list_stack_t *stack = list_stack;
    token_t *token;

    if (stack->depth == 0)
        return false;
    puts("Стек-список:");
    for (size_t i = 0; i < stack->depth; i++)
    {
        token = see_elem_list(stack, i);
        printf("%p ", (void *)token);
        print_token(token);
    }
    return true;
}

bool push_list(void *list_stack, token_t value)
{
    list_stack_t *stack = list_stack;
    list_node_t *old_head;

    if (stack->depth == STACK_DEPTH)
        return false;
    old_head = stack->head;
    stack->head = malloc(sizeof(list_node_t));
    stack->head->next_node = old_head;
    stack->head->value = value;
    stack->depth++;
    return true;
}

bool pop_list(void *list_stack, token_t *value)
{
    list_stack_t *stack = list_stack;
    list_node_t *new_head;

    if (stack->depth == 0)
        return false;

    new_head = stack->head->next_node;
    *value = stack->head->value;
    free(stack->head);
    stack->head = new_head;
    stack->depth--;
    return true;
}


bool pop_list_mem(void *list_stack, token_t *value, freed_nodes_array_t *freed_memory)
{
    list_stack_t *stack = list_stack;
    if (stack->depth == 0)
        return false;
    freed_memory->freed_nodes[(freed_memory->len)++] = stack->head;
    pop_list(list_stack, value);
    return true;
}

void print_freed(freed_nodes_array_t *freed_nodes_array)
{
    if (freed_nodes_array->len == 0)
    {
        puts("Массив адресов освобожденной памяти пуст!");
        return;
    }
    puts("Массив адресов освобожденной памяти:");
    for (size_t i = 0; i < freed_nodes_array->len; i++)
        printf("%p\n", (void *)freed_nodes_array->freed_nodes[i]);
}

void free_list(void *list_stack)
{
    list_stack_t *stack = list_stack;
    list_node_t *node;

    for (size_t i = 0; i < stack->depth; i++)
    {
        node = stack->head->next_node;
        free(stack->head);
        stack->head = node;
    }
    free(stack);
}

void free_debug_list(void *list_stack, freed_nodes_array_t *freed_memory)
{
    free(freed_memory);
    free_list(list_stack);
}

void init_list(stack_t *stack)
{
    list_stack_t *list_stack;
    if (stack->stack != NULL)
        stack->free(stack->stack);

    list_stack = malloc(sizeof(list_stack_t));
    list_stack->depth = 0;
    stack->stack = list_stack;
    stack->free = free_list;
    stack->push = push_list;
    stack->pop = pop_list;
}

void init_list_debug(debug_stack_t *debug_stack)
{
    list_stack_t *list_stack;
    freed_nodes_array_t *freed_nodes_array;
    if (debug_stack->stack != NULL)
        debug_stack->free(debug_stack->stack, debug_stack->freed_memory);

    list_stack = malloc(sizeof(list_stack_t));
    list_stack->depth = 0;
    debug_stack->stack = list_stack;
    freed_nodes_array = malloc(sizeof(freed_nodes_array_t));
    freed_nodes_array->len = 0;
    debug_stack->freed_memory = freed_nodes_array;
    debug_stack->free = free_debug_list;
    debug_stack->push = push_list;
    debug_stack->pop_mem = pop_list_mem;
    debug_stack->print = print_list;
    debug_stack->print_freed = print_freed;
}

