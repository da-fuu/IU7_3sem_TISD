#ifndef LIST_QUEUE_H__
#define LIST_QUEUE_H__

#include "struct.h"


bool push_list(void *list_queue, value_t value);

bool pop_list(void *list_queue, value_t *value);

bool pop_list_mem(void *list_queue, value_t *value, list_queue_t *freed_memory);

void print_freed(list_queue_t *freed_nodes_array);

bool print_list(void *list_queue);

void free_list(void *list_queue);

void free_debug_list(void *list_queue, list_queue_t *freed_memory);

void init_list(abstract_queue_t *debug_queue);

void init_list_debug(debug_queue_t *debug_queue);


#endif // #ifndef LIST_QUEUE_H__
