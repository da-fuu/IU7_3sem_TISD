#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hashtable.h"
#include "open_hashtable.h"


char *abs_open_hashtable_find(void *data, reserved_word_t reserved_word, int *n_comps)
{
    open_hashtable_t *hashtable = data;
    int hash = hash_function(reserved_word, hashtable->size);
    open_ht_node_t *ht_node = hashtable->array[hash];

    if (ht_node == NULL)
        return NULL;
    while (ht_node != NULL)
    {
        ++*n_comps;
        if (strcmp(ht_node->reserved_word_info.word, reserved_word) == 0)
            break;
        ht_node = ht_node->next;
    }
    if (ht_node == NULL)
        return NULL;
    return ht_node->reserved_word_info.help;
}


static open_ht_node_t *insert_node(open_ht_node_t *node, reserved_word_info_t *reserved_word_info)
{
    open_ht_node_t *new = malloc(sizeof(open_ht_node_t));
    new->next = NULL;
    new->reserved_word_info = *reserved_word_info;
    
    if (node != NULL)
        node->next = new;
    
    return new;
}

bool abs_open_hashtable_insert(void *data, reserved_word_info_t *reserved_word_info, int *n_comps)
{
    open_hashtable_t *hashtable = data;
    int hash = hash_function(reserved_word_info->word, hashtable->size);
    open_ht_node_t *ht_node = hashtable->array[hash];

    ++*n_comps;
    if (ht_node == NULL)
    {
        hashtable->array[hash] = insert_node(hashtable->array[hash], reserved_word_info);
        return true;
    }
    
    while (ht_node->next != NULL)
    {      
        ++*n_comps;
        if (strcmp(ht_node->reserved_word_info.word, reserved_word_info->word) == 0)
            return false;
        ht_node = ht_node->next;
    }
    ++*n_comps;
    if (strcmp(ht_node->reserved_word_info.word, reserved_word_info->word) == 0)
        return false;
    
    insert_node(ht_node, reserved_word_info);
    
    return true;
}


static bool remove_from_list(open_ht_node_t **head, reserved_word_t word, int *n_comps)
{
    open_ht_node_t *temp, *curr;
    ++*n_comps;
    if (strcmp((*head)->reserved_word_info.word, word) == 0)
    {
        temp = (*head)->next;
        free((*head)->reserved_word_info.help);
        free(*head);
        *head = temp;
        return true;
    }
    curr = *head;
    while (curr->next != NULL)
    {
        ++*n_comps;
        if (strcmp(curr->next->reserved_word_info.word, word) == 0)
        {
            temp = curr->next->next;
            free(curr->next->reserved_word_info.help);
            free(curr->next);
            curr->next = temp;
            return true;
        }
        curr = curr->next;
    }
    return false;
}

bool abs_open_hashtable_remove(void *data, reserved_word_t word, int *n_comps)
{
    open_hashtable_t *hashtable = data;
    int hash = hash_function(word, hashtable->size);
    if (hashtable->array[hash] != NULL)
        return remove_from_list(&hashtable->array[hash], word, n_comps);
    return false;
}


void free_open_ht_node(open_ht_node_t *node, bool all)
{
    open_ht_node_t *temp;
    while (node != NULL)
    {
        temp = node->next;
        if (all)
            free(node->reserved_word_info.help);
        free(node);
        node = temp;
    }
}


void open_hashtable_free(open_ht_node_t **array, int size, bool all)
{
    for (int i = 0; i < size; i++)
        free_open_ht_node(array[i], all);
    free(array);
}

void abs_open_hashtable_free(void *data)
{
    open_hashtable_t **open_hashtable = data;

    open_hashtable_free((*open_hashtable)->array, (*open_hashtable)->size, true);
    free(*open_hashtable);
    *open_hashtable = NULL;
}


void open_hashtable_restruct(open_hashtable_t *open_hashtable, int new_size)
{
    open_hashtable_t old_hashtable;
    open_ht_node_t *node;
    int tmp = 0;
    old_hashtable.array = open_hashtable->array;
    old_hashtable.size = open_hashtable->size;
    old_hashtable.max_comparisons = open_hashtable->max_comparisons;

    open_hashtable->size = new_size;
    open_hashtable->array = malloc(open_hashtable->size * sizeof(open_ht_node_t *));
    for (int i = 0; i < open_hashtable->size; i++)
        open_hashtable->array[i] = NULL;
    
    for (int i = 0; i < old_hashtable.size; i++)
    {
        node = old_hashtable.array[i];
        while (node != NULL)
        {
            abs_open_hashtable_insert(open_hashtable, &node->reserved_word_info, &tmp);
            node = node->next;
        }
    }
    open_hashtable_free(old_hashtable.array, old_hashtable.size, false);
}

void abs_open_hashtable_restruct(void *data, int comps)
{
    open_hashtable_t *hashtable = data;
    if (comps <= hashtable->max_comparisons)
        return;

    puts("Превышено число сравнений, произведена реструктуризация");
    open_hashtable_restruct(hashtable, find_next_size(hashtable->size));
}

bool abs_open_hashtable_set_comps(void *data, int size)
{
    open_hashtable_t *open_hashtable = data;
    if (size >= 0)
        open_hashtable->max_comparisons = size;
    return true;
}

void abs_open_hashtable_clean_set_size(void *data, int size)
{
    open_hashtable_t *open_hashtable = data;
    open_hashtable_free(open_hashtable->array, open_hashtable->size, true);
    
    open_hashtable->size = find_next_size(size);
    open_hashtable->array = malloc(open_hashtable->size * sizeof(open_ht_node_t *));
    for (int i = 0; i < open_hashtable->size; i++)
        open_hashtable->array[i] = NULL;
}

void abs_open_hashtable_print(void *data)
{
    open_ht_node_t *node;
    open_hashtable_t *open_hashtable = data;
    for (int i = 0; i < open_hashtable->size; i++)
    {
        node = open_hashtable->array[i];
        if (node == NULL)
        {
            printf("NULL\n");
            continue;
        }
        while (node != NULL)
        {
            printf("%s -> ", node->reserved_word_info.word);
            node = node->next;
        }
        printf("\n");
    }
}

void init_open_hashtable(assoc_array_t *assoc_array)
{
    if (assoc_array->data != NULL)
        assoc_array->free(&assoc_array->data);
    
    open_hashtable_t *open_hashtable = malloc(sizeof(open_hashtable_t));
    open_hashtable->size = find_next_size(INITIAL_SIZE);
    open_hashtable->max_comparisons = MAX_COMPARISONS;
    open_hashtable->array = malloc(open_hashtable->size * sizeof(open_ht_node_t *));
    for (int i = 0; i < open_hashtable->size; i++)
        open_hashtable->array[i] = NULL;
    
    assoc_array->data = open_hashtable;

    assoc_array->find = abs_open_hashtable_find;
    assoc_array->insert = abs_open_hashtable_insert;
    assoc_array->remove = abs_open_hashtable_remove;
    assoc_array->print = abs_open_hashtable_print;
    assoc_array->set_comps = abs_open_hashtable_set_comps;
    assoc_array->restruct = abs_open_hashtable_restruct;
    assoc_array->clean_and_set_size = abs_open_hashtable_clean_set_size;
    assoc_array->free = abs_open_hashtable_free;
}
