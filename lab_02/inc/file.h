#ifndef FILE_H__
#define FILE_H__

#include <stdbool.h>
#include "struct.h"


void read_table(table_t *table);

void save_table(const table_t *table);

bool read_num(int *num, int max_val, FILE *file);

bool read_line(char *str, int len, FILE *file);

bool read_from_file(table_t *table, FILE *file);

void flush_stdin(void);

#endif // #ifndef FILE_H__
