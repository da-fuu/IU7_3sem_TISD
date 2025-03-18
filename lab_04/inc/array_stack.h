#ifndef ARRAY_STACK_H__
#define ARRAY_STACK_H__

#include "struct.h"


bool push_array(void *array_stack, token_t value);

bool pop_array(void *array_stack, token_t *value);


bool pop_array_mem_stub(void *array_stack, token_t *value, freed_nodes_array_t *freed_memory);

void print_freed_stub(freed_nodes_array_t *freed_nodes_array);

bool print_array(void *array_stack);

void free_array(void *array_stack);

void free_debug_array_stub(void *array_stack, freed_nodes_array_t *freed_memory);

void init_array(stack_t *stack);

void init_array_debug(debug_stack_t *debug_stack);

#endif // #ifndef ARRAY_STACK_H__
