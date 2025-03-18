#include <string.h>
#include <stdlib.h>
#include "struct.h"
#include "sort.h"


static int comparator_subscribers(const void *a, const void *b)
{
    const subscriber_t *sub_a = a;
    const subscriber_t *sub_b = b;

    return strcmp(sub_a->lastname, sub_b->lastname);
}

static int comparator_keys(const void *a, const void *b)
{
    const key_elem_t *key_a = a;
    const key_elem_t *key_b = b;

    return strcmp(key_a->value, key_b->value);
}

void fast_sort_table(void *input)
{
    table_t *table = input;
    qsort(table->subscribers, table->len, sizeof(table->subscribers[0]), comparator_subscribers);
}

void fast_sort_key_table(void *input)
{
    key_table_t *key_table = input;
    qsort(key_table->keys, key_table->len, sizeof(key_table->keys[0]), comparator_keys);
}

void slow_sort_table(void *input)
{
    subscriber_t tmp;
    table_t *table = input;
    for (int i = 0; i < table->len - 1; i++)
        for (int j = 0; j < table->len - 1 - i; j++)
            if (comparator_subscribers(&table->subscribers[j], &table->subscribers[j + 1]) > 0)
            {
                tmp = table->subscribers[j + 1];
                table->subscribers[j + 1] = table->subscribers[j];
                table->subscribers[j] = tmp;
            }
}

void slow_sort_key_table(void *input)
{
    key_elem_t tmp;
    key_table_t *key_table = input;

    for (int i = 0; i < key_table->len - 1; i++)
        for (int j = 0; j < key_table->len - 1 - i; j++)
            if (comparator_keys(&key_table->keys[j], &key_table->keys[j + 1]) > 0)
            {
                tmp = key_table->keys[j + 1];
                key_table->keys[j + 1] = key_table->keys[j];
                key_table->keys[j] = tmp;
            }
}
