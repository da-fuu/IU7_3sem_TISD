#ifndef SPARSE_MATRIX_H__
#define SPARSE_MATRIX_H__

#include "struct.h"


void sparse_to_standard(both_matrix_t matrix);

void sum_sparse(sparse_matrix_t *in_matrix_1, sparse_matrix_t *in_matrix_2, sparse_matrix_t *out_matrix);


#endif // #ifndef SPARSE_MATRIX_H__
