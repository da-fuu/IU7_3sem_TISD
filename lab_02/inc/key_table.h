#ifndef KEY_TABLE_H__
#define KEY_TABLE_H__

#include "struct.h"


void create_key_table(const table_t *table, key_table_t *key_table);

void create_print_key_table(const table_t *table, key_table_t *key_table);

void print_sorted_key_table(const table_t *table, key_table_t *key_table);

void sort_table_with_key_table(const table_t *table, key_table_t *key_table);


#endif // #ifndef KEY_TABLE_H__
