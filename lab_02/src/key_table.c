#include <string.h>
#include "sort.h"
#include "print.h"
#include "key_table.h"


void create_key_table(const table_t *table, key_table_t *key_table)
{
    key_table->len = table->len;

    for (int i = 0; i < table->len; i++)
    {
        strcpy(key_table->keys[i].value, table->subscribers[i].lastname);
        key_table->keys[i].index = i;
    }
}

void create_print_key_table(const table_t *table, key_table_t *key_table)
{
    create_key_table(table, key_table);
    print_key_table(key_table);
}

void print_sorted_key_table(const table_t *table, key_table_t *key_table)
{
    create_key_table(table, key_table);
    fast_sort_key_table(key_table);
    print_key_table(key_table);
}

static void print_table_with_key_table(const table_t *table, const key_table_t *key_table)
{
    int index;
    for (int i = 0; i < key_table->len; i++)
    {
        index = key_table->keys[i].index;
        print_subscriber(&table->subscribers[index], index);
    }
}

void sort_table_with_key_table(const table_t *table, key_table_t *key_table)
{
    create_key_table(table, key_table);
    fast_sort_key_table(key_table);
    print_table_with_key_table(table, key_table);
}
