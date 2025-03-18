#include <stdio.h>
#include <malloc.h>

#include "array_queue.h"


bool pop_array_mem_stub(void *array_queue, value_t *value, list_queue_t *freed_memory)
{
    (void)freed_memory;
    return pop_array(array_queue, value);
}

void print_freed_stub(list_queue_t *freed_nodes_array)
{
    (void)freed_nodes_array;
    puts("Действие не имеет смысла для очереди-массива!");
}

bool pop_array(void *array_queue, value_t *value)
{
    array_queue_t *queue = array_queue;
    
    if (queue->depth == 0)
        return false;
    *value = queue->values[queue->out];
    
    queue->depth--;
    if (queue->depth != 0)
        queue->out = (queue->out + 1) % QUEUE_DEPTH;
    return true;
}

bool push_array(void *array_queue, value_t value)
{
    array_queue_t *queue = array_queue;

    if (queue->depth == QUEUE_DEPTH)
        return false;
    if (queue->depth == 0)
    {
        queue->in = 0;
        queue->out = 0;
    }
    queue->values[queue->in] = value;
    queue->in = (queue->in + 1) % QUEUE_DEPTH;
    queue->depth++;
    return true;
}

static value_t *see_elem_array(array_queue_t *array_queue, size_t ind)
{
    return &array_queue->values[(array_queue->out + ind) % QUEUE_DEPTH];
}

bool print_array(void *array_queue)
{
    array_queue_t *queue = array_queue;
    value_t *value;

    if (queue->depth == 0)
        return false;
    puts("Очередь-массив:");
    for (int i = 0; i < queue->depth; i++)
    {
        value = see_elem_array(queue, i);
        printf("%p %lf\n", (void *)value, value->time);
    }
    return true;
}

int get_len_array(void *queue)
{
    array_queue_t *array_queue = queue;
    return array_queue->depth;
}

void free_array(void *array_queue)
{
    array_queue_t *queue = array_queue;
    free(queue);
}

void free_debug_array_stub(void *array_queue, list_queue_t *freed_memory)
{
    (void)freed_memory;
    free_array(array_queue);
}

void init_array(abstract_queue_t *queue)
{
    array_queue_t *array_queue;
    if (queue->queue != NULL)
        queue->free(queue->queue);

    array_queue = malloc(sizeof(array_queue_t));
    array_queue->depth = 0;
    queue->queue = array_queue;
    queue->push = push_array;
    queue->pop = pop_array;
    queue->get_len = get_len_array;
    queue->free = free_array;
}

void init_array_debug(debug_queue_t *debug_queue)
{
    array_queue_t *array_queue;
    if (debug_queue->queue != NULL)
        debug_queue->free(debug_queue->queue, debug_queue->freed_memory);

    array_queue = malloc(sizeof(array_queue_t));
    array_queue->depth = 0;
    
    debug_queue->queue = array_queue;
    debug_queue->freed_memory = NULL;
    debug_queue->free = free_debug_array_stub;
    debug_queue->push = push_array;
    debug_queue->pop_mem = pop_array_mem_stub;
    debug_queue->print = print_array;
    debug_queue->print_freed = print_freed_stub;
}
