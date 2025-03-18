#include <stdio.h>
#include <malloc.h>

#include "print.h"
#include "array_stack.h"

bool pop_array_mem_stub(void *array_stack, token_t *value, freed_nodes_array_t *freed_memory)
{
    (void)freed_memory;
    return pop_array(array_stack, value);
}

void print_freed_stub(freed_nodes_array_t *freed_nodes_array)
{
    (void)freed_nodes_array;
    puts("Действие не имеет смысла для стека-массива!");
}

bool pop_array(void *array_stack, token_t *value)
{
    array_stack_t *stack = array_stack;
    if (stack->depth == 0)
        return false;
    *value = stack->tokens[--(stack->depth)];
    return true;
}

bool push_array(void *array_stack, token_t value)
{
    array_stack_t *stack = array_stack;

    if (stack->depth == STACK_DEPTH)
        return false;
    stack->tokens[(stack->depth)++] = value;
    return true;
}

static token_t *see_elem_array(array_stack_t *array_stack, size_t ind)
{
    return &array_stack->tokens[ind];
}

bool print_array(void *array_stack)
{
    array_stack_t *stack = array_stack;
    token_t *token;

    if (stack->depth == 0)
        return false;
    puts("Стек-массив:");
    for (size_t i = stack->depth; i > 0; i--)
    {
        token = see_elem_array(stack, i - 1);
        print_token(token);
    }
    return true;
}

void free_array(void *array_stack)
{
    array_stack_t *stack = array_stack;
    free(stack);
}

void free_debug_array_stub(void *array_stack, freed_nodes_array_t *freed_memory)
{
    (void)freed_memory;
    free_array(array_stack);
}

void init_array(stack_t *stack)
{
    array_stack_t *array_stack;
    if (stack->stack != NULL)
        stack->free(stack->stack);

    array_stack = malloc(sizeof(array_stack_t));
    array_stack->depth = 0;
    stack->stack = array_stack;
    stack->push = push_array;
    stack->pop = pop_array;
    stack->free = free_array;
}

void init_array_debug(debug_stack_t *debug_stack)
{
    array_stack_t *array_stack;
    if (debug_stack->stack != NULL)
        debug_stack->free(debug_stack->stack, debug_stack->freed_memory);

    array_stack = malloc(sizeof(array_stack_t));
    array_stack->depth = 0;
    debug_stack->stack = array_stack;
    debug_stack->freed_memory = NULL;
    debug_stack->free = free_debug_array_stub;
    debug_stack->push = push_array;
    debug_stack->pop_mem = pop_array_mem_stub;
    debug_stack->print = print_array;
    debug_stack->print_freed = print_freed_stub;
}
