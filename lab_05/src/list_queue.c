#include <stdio.h>
#include <malloc.h>

#include "list_queue.h"


static value_t *see_elem_list(list_queue_t *queue, size_t ind)
{
    list_node_t *node = queue->head;
    for (size_t i = 0; i < ind; i++)
        node = node->next_node;
    return &node->value;
}

bool print_list(void *list_queue)
{
    list_queue_t *queue = list_queue;
    value_t *value;

    if (queue->depth == 0)
        return false;
    puts("Очередь-список:");
    for (int i = 0; i < queue->depth; i++)
    {
        value = see_elem_list(queue, i);
        printf("%p %lf\n", (void *)value, value->time);
    }
    return true;
}

bool push_list(void *list_queue, value_t value)
{
    list_queue_t *queue = list_queue;
    list_node_t *curr;

    if (queue->depth == 0)
    {
        curr = malloc(sizeof(list_node_t));
        if (curr == NULL)
            return false;
        queue->head = curr;
        queue->tail = curr;
    }
    else
    {
        queue->tail->next_node = malloc(sizeof(list_node_t));
        if (queue->tail->next_node == NULL)
            return false;
        queue->tail = queue->tail->next_node;
        curr = queue->tail;
    }
    curr->next_node = NULL;
    curr->value = value;
    queue->depth++;
    return true;
}

bool pop_list(void *list_queue, value_t *value)
{
    list_queue_t *queue = list_queue;
    list_node_t *new_head;

    if (queue->depth == 0)
        return false;

    new_head = queue->head->next_node;
    *value = queue->head->value;
    free(queue->head);
    queue->head = new_head;
    queue->depth--;
    return true;
}


bool pop_list_mem(void *list_queue, value_t *value, list_queue_t *freed_memory)
{
    list_queue_t *queue = list_queue;
    value_t ptr;
    if (queue->depth == 0)
        return false;
    ptr.ptr = queue->head;
    push_list(freed_memory, ptr);
    pop_list(list_queue, value);
    return true;
}

void print_freed(list_queue_t *freed_nodes_list)
{
    value_t *value;

    if (freed_nodes_list->depth == 0)
    {
        puts("Массив адресов освобожденной памяти пуст!");
        return;
    }
    puts("Список адресов освобожденной памяти:");
    for (int i = 0; i < freed_nodes_list->depth; i++)
    {
        value = see_elem_list(freed_nodes_list, i);
        printf("%p\n", (void *)value->ptr);
    }
}

void free_list(void *list_queue)
{
    list_queue_t *queue = list_queue;
    list_node_t *node;

    for (int i = 0; i < queue->depth; i++)
    {
        node = queue->head->next_node;
        free(queue->head);
        queue->head = node;
    }
    free(queue);
}

void free_debug_list(void *list_queue, list_queue_t *freed_memory)
{
    free_list(freed_memory);
    free_list(list_queue);
}

int get_len_list(void *queue)
{
    list_queue_t *list_queue = queue;
    return list_queue->depth;
}

void init_list(abstract_queue_t *queue)
{
    list_queue_t *list_queue;
    if (queue->queue != NULL)
        queue->free(queue->queue);

    list_queue = malloc(sizeof(list_queue_t));
    list_queue->depth = 0;
    queue->queue = list_queue;
    queue->free = free_list;
    queue->push = push_list;
    queue->pop = pop_list;
    queue->get_len = get_len_list;
}

void init_list_debug(debug_queue_t *debug_queue)
{
    list_queue_t *list_queue;
    list_queue_t *freed_nodes_list;
    
    if (debug_queue->queue != NULL)
        debug_queue->free(debug_queue->queue, debug_queue->freed_memory);

    list_queue = malloc(sizeof(list_queue_t));
    list_queue->depth = 0;
    debug_queue->queue = list_queue;
    freed_nodes_list = malloc(sizeof(list_queue_t));
    freed_nodes_list->depth = 0;
    debug_queue->freed_memory = freed_nodes_list;
    debug_queue->free = free_debug_list;
    debug_queue->push = push_list;
    debug_queue->pop_mem = pop_list_mem;
    debug_queue->print = print_list;
    debug_queue->print_freed = print_freed;
}

