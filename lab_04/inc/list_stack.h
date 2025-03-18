#ifndef LIST_STACK_H__
#define LIST_STACK_H__

#include "struct.h"


bool push_list(void *list_stack, token_t value);

bool pop_list(void *list_stack, token_t *value);


bool pop_list_mem(void *list_stack, token_t *value, freed_nodes_array_t *freed_memory);

void print_freed(freed_nodes_array_t *freed_nodes_array);

bool print_list(void *list_stack);

void free_list(void *list_stack);

void free_debug_list(void *list_stack, freed_nodes_array_t *freed_memory);

void init_list(stack_t *debug_stack);

void init_list_debug(debug_stack_t *debug_stack);

#endif // #ifndef LIST_STACK_H__
