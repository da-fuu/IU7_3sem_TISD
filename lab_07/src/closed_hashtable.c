#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hashtable.h"
#include "closed_hashtable.h"


char *abs_closed_hashtable_find(void *data, reserved_word_t reserved_word, int *n_comps)
{
    closed_hashtable_t *hashtable = data;
    int hash = hash_function(reserved_word, hashtable->size);
    closed_ht_elem_t ht_elem = hashtable->array[hash];

    while (ht_elem.state != FREE)
    {
        if (ht_elem.state == USED)
        {
            ++*n_comps;
            if (strcmp(ht_elem.reserved_word_info.word, reserved_word) == 0)
                return ht_elem.reserved_word_info.help;
        }
        hash = (hash + 1) % hashtable->size;
        ht_elem = hashtable->array[hash];
    }
    return NULL;
}

bool abs_closed_hashtable_insert(void *data, reserved_word_info_t *reserved_word_info, int *n_comps)
{
    closed_hashtable_t *hashtable = data;
    int hash = hash_function(reserved_word_info->word, hashtable->size);
    int ind = hash;
    
    ++*n_comps;
    while (hashtable->array[ind].state == USED)
    {
        ++*n_comps;
        if (strcmp(hashtable->array[ind].reserved_word_info.word, reserved_word_info->word) == 0)
            return false;
        ind = (ind + 1) % hashtable->size;
        if (ind == hash)
            return false;
    }
    
    hashtable->array[ind].state = USED;
    hashtable->array[ind].reserved_word_info = *reserved_word_info;
    
    return true;
}


bool abs_closed_hashtable_remove(void *data, reserved_word_t word, int *n_comps)
{
    closed_hashtable_t *hashtable = data;
    int hash = hash_function(word, hashtable->size);
    int ind = hash;

    while (hashtable->array[ind].state != FREE)
    {
        if (hashtable->array[ind].state == USED)
        {
            ++*n_comps;
            if (strcmp(hashtable->array[ind].reserved_word_info.word, word) == 0)
            {
                hashtable->array[ind].state = DELETED;
                free(hashtable->array[ind].reserved_word_info.help);
                return true;
            }
        }
        ind = (ind + 1) % hashtable->size;
        if (ind == hash)
            break;
    }
    return false;
}

void closed_hashtable_free(closed_ht_elem_t *array, int size, bool all)
{
    if (all)
        for (int i = 0; i < size; i++)
            if (array[i].state == USED)
                free(array[i].reserved_word_info.help);
    free(array);
}

void abs_closed_hashtable_free(void *data)
{
    closed_hashtable_t **closed_hashtable = data;

    closed_hashtable_free((*closed_hashtable)->array, (*closed_hashtable)->size, true);
    free(*closed_hashtable);
    *closed_hashtable = NULL;
}


void closed_hashtable_restruct(closed_hashtable_t *closed_hashtable, int new_size)
{
    closed_hashtable_t old_hashtable;
    int tmp = 0;
    old_hashtable.array = closed_hashtable->array;
    old_hashtable.size = closed_hashtable->size;
    old_hashtable.max_comparisons = closed_hashtable->max_comparisons;

    closed_hashtable->size = new_size;
    closed_hashtable->array = malloc(closed_hashtable->size * sizeof(closed_ht_elem_t));
    for (int i = 0; i < closed_hashtable->size; i++)
        closed_hashtable->array[i].state = FREE;

    for (int i = 0; i < old_hashtable.size; i++)
    {
        if (old_hashtable.array[i].state == USED)
            abs_closed_hashtable_insert(closed_hashtable, &old_hashtable.array[i].reserved_word_info, &tmp);
    }
    closed_hashtable_free(old_hashtable.array, old_hashtable.size, false);
}

bool abs_closed_hashtable_set_comps(void *data, int size)
{
    closed_hashtable_t *closed_hashtable = data;
    if (size >= 0)
        closed_hashtable->max_comparisons = size;
    return true;
}

void abs_closed_hashtable_clean_set_size(void *data, int size)
{
    closed_hashtable_t *closed_hashtable = data;
    closed_hashtable_free(closed_hashtable->array, closed_hashtable->size, true);

    closed_hashtable->size = find_next_size(size);
    closed_hashtable->array = malloc(closed_hashtable->size * sizeof(closed_ht_elem_t));
    for (int i = 0; i < closed_hashtable->size; i++)
        closed_hashtable->array[i].state = FREE;
}

void abs_closed_hashtable_print(void *data)
{
    closed_ht_elem_t elem;
    closed_hashtable_t *closed_hashtable = data;
    printf("Индекс | Хеш | Слово\n");
    for (int i = 0; i < closed_hashtable->size; i++)
    {
        elem = closed_hashtable->array[i];
        switch (elem.state)
        {
            case FREE:
                printf(" %5d |  -  | -\n", i);
                break;
            case USED:
                printf(" %5d | %3d | %s\n", i, hash_function(elem.reserved_word_info.word, closed_hashtable->size), elem.reserved_word_info.word);
                break;
            case DELETED:
                printf(" %5d | %3d | Элемент удален\n", i, hash_function(elem.reserved_word_info.word, closed_hashtable->size));
                break;
        }
    }
}

void abs_closed_hashtable_restruct(void *data, int comps)
{
    closed_hashtable_t *hashtable = data;
    if (comps <= hashtable->max_comparisons)
        return;

    puts("Превышено число сравнений, произведена реструктуризация");
    closed_hashtable_restruct(hashtable, find_next_size(hashtable->size));
}


void init_closed_hashtable(assoc_array_t *assoc_array)
{
    if (assoc_array->data != NULL)
        assoc_array->free(&assoc_array->data);
    
    closed_hashtable_t *closed_hashtable = malloc(sizeof(closed_hashtable_t));
    closed_hashtable->size = find_next_size(INITIAL_SIZE);
    closed_hashtable->max_comparisons = MAX_COMPARISONS;
    closed_hashtable->array = malloc(closed_hashtable->size * sizeof(closed_ht_elem_t));
    for (int i = 0; i < closed_hashtable->size; i++)
        closed_hashtable->array[i].state = FREE;

    assoc_array->data = closed_hashtable;

    assoc_array->find = abs_closed_hashtable_find;
    assoc_array->insert = abs_closed_hashtable_insert;
    assoc_array->remove = abs_closed_hashtable_remove;
    assoc_array->print = abs_closed_hashtable_print;
    assoc_array->set_comps = abs_closed_hashtable_set_comps;
    assoc_array->clean_and_set_size = abs_closed_hashtable_clean_set_size;
    assoc_array->restruct = abs_closed_hashtable_restruct;
    assoc_array->free = abs_closed_hashtable_free;
}
