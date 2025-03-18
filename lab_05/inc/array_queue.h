#ifndef ARRAY_QUEUE_H__
#define ARRAY_QUEUE_H__

#include "struct.h"


bool push_array(void *array_queue, value_t value);

bool pop_array(void *array_queue, value_t *value);

bool pop_array_mem_stub(void *array_queue, value_t *value, list_queue_t *freed_memory);

void print_freed_stub(list_queue_t *freed_nodes_array);

bool print_array(void *array_queue);

void free_array(void *array_queue);

void free_debug_array_stub(void *array_queue, list_queue_t *freed_memory);

void init_array(abstract_queue_t *queue);

void init_array_debug(debug_queue_t *debug_queue);


#endif // #ifndef ARRAY_QUEUE_H__
