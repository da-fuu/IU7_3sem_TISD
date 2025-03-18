#ifndef INPUT_H__
#define INPUT_H__

#include <stdbool.h>
#include "struct.h"


void flush_stdin(void);

bool read_dimensions(int *rows, int *cols, bool verbose, FILE *file);

void init_matrix(both_matrix_t matrix);

bool read_num(int *num, int max_val, FILE *file);

void read_file(both_matrix_t matrix_1, both_matrix_t matrix_2);

void read_user_standard(both_matrix_t matrix_1, both_matrix_t matrix_2);

void read_user_coord(both_matrix_t matrix_1, both_matrix_t matrix_2);


#endif // #ifndef INPUT_H__
