#ifndef STANDARD_MATRIX_H__
#define STANDARD_MATRIX_H__

#include "struct.h"


void standard_to_sparse(both_matrix_t matrix);

void fill_with_sparsity(both_matrix_t matrix_1, both_matrix_t matrix_2);

void sum_standard(const standard_matrix_t *in_matrix_1, const standard_matrix_t *in_matrix_2, standard_matrix_t *out_matrix);

void fill_standard_with_zeroes(standard_matrix_t *matrix);

void fill_standard(standard_matrix_t *standard_matrix, int nonzero_elems);


#endif // #ifndef STANDARD_MATRIX_H__
