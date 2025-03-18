#ifndef PRINT_H__
#define PRINT_H__

#include "struct.h"


void print_table(const table_t *table);

void print_sorted_table(table_t *table);

void print_key_table(const key_table_t *key_table);

void print_subscriber(const subscriber_t *subscriber, int index);

void print_header(void);


#endif // #ifndef PRINT_H__
