#include <stdio.h>
#include "print.h"

void print_standard(const standard_matrix_t *standard_matrix)
{
    for (int i = 0; i < standard_matrix->rows; i++)
    {
        for (int j = 0; j < standard_matrix->cols; j++)
            printf("%5d ", standard_matrix->matrix[i][j]);
        printf("\n");
    }
}

void print_sparse(const sparse_matrix_t *sparse_matrix)
{
    printf("A: ");
    for (int i = 0; i < sparse_matrix->elements_num; i++)
    {
        printf("%d ", sparse_matrix->a[i]);
    }
    printf("\n");
    printf("IA: ");
    for (int i = 0; i < sparse_matrix->elements_num; i++)
    {
        printf("%d ", sparse_matrix->ia[i]);
    }
    printf("\n");
    printf("JA: ");
    for (int i = 0; i < sparse_matrix->cols; i++)
    {
        printf("%d ", sparse_matrix->ja[i]);
    }
    printf("\n");
}
