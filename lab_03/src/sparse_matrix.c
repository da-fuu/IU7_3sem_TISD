#include <stdbool.h>
#include "standard_matrix.h"
#include "sparse_matrix.h"


static int find_after_last_in_col(sparse_matrix_t *sparse_matrix, int column)
{
    if (sparse_matrix->ja[column] < 0)
        return sparse_matrix->ja[column];
    do
    {
        column++;
        if (column == sparse_matrix->cols)
            return sparse_matrix->elements_num;
    }
    while (sparse_matrix->ja[column] < 0);

    return sparse_matrix->ja[column];
}

void sparse_to_standard(both_matrix_t matrix)
{
    int i;
    int index_nonzero;
    int last_in_col;
    matrix.standard_matrix->rows = matrix.sparse_matrix->rows;
    matrix.standard_matrix->cols = matrix.sparse_matrix->cols;

    fill_standard_with_zeroes(matrix.standard_matrix);

    for (int j = 0; j < matrix.sparse_matrix->cols; j++)
    {
        index_nonzero = matrix.sparse_matrix->ja[j];
        last_in_col = find_after_last_in_col(matrix.sparse_matrix, j);
        while (index_nonzero < last_in_col)
        {
            i = matrix.sparse_matrix->ia[index_nonzero];
            matrix.standard_matrix->matrix[i][j] = matrix.sparse_matrix->a[index_nonzero];
            index_nonzero++;
        }
    }
}

void sum_sparse(sparse_matrix_t *in_matrix_1, sparse_matrix_t *in_matrix_2, sparse_matrix_t *out_matrix)
{
    int i1, i2;
    int nnz1, nnz2;
    bool flag;
    int last_in_col1, last_in_col2;
    out_matrix->rows = in_matrix_1->rows;
    out_matrix->cols = in_matrix_1->cols;

    out_matrix->elements_num = 0;

    for (int j = 0; j < out_matrix->cols; j++)
    {
        flag = false;
        nnz1 = in_matrix_1->ja[j];
        nnz2 = in_matrix_2->ja[j];
        last_in_col1 = find_after_last_in_col(in_matrix_1, j);
        last_in_col2 = find_after_last_in_col(in_matrix_2, j);

        while (nnz1 < last_in_col1 && nnz2 < last_in_col2)
        {
            i1 = in_matrix_1->ia[nnz1];
            i2 = in_matrix_2->ia[nnz2];
            if (i1 < i2)
            {
                out_matrix->a[out_matrix->elements_num] = in_matrix_1->a[nnz1];
                out_matrix->ia[out_matrix->elements_num] = i1;
                if (!flag)
                {
                    out_matrix->ja[j] = out_matrix->elements_num;
                    flag = true;
                }
                out_matrix->elements_num++;
                nnz1++;
            }
            else if (i1 > i2)
            {
                out_matrix->a[out_matrix->elements_num] = in_matrix_2->a[nnz2];
                out_matrix->ia[out_matrix->elements_num] = i2;
                if (!flag)
                {
                    out_matrix->ja[j] = out_matrix->elements_num;
                    flag = true;
                }
                out_matrix->elements_num++;
                nnz2++;
            }
            else
            {
                out_matrix->a[out_matrix->elements_num] = in_matrix_1->a[nnz1] + in_matrix_2->a[nnz2];
                out_matrix->ia[out_matrix->elements_num] = i1;
                if (!flag)
                {
                    out_matrix->ja[j] = out_matrix->elements_num;
                    flag = true;
                }
                out_matrix->elements_num++;
                nnz1++;
                nnz2++;
            }
        }

        while (nnz1 < last_in_col1)
        {
            i1 = in_matrix_1->ia[nnz1];
            out_matrix->a[out_matrix->elements_num] = in_matrix_1->a[nnz1];
            out_matrix->ia[out_matrix->elements_num] = i1;
            if (!flag)
            {
                out_matrix->ja[j] = out_matrix->elements_num;
                flag = true;
            }
            out_matrix->elements_num++;
            nnz1++;
        }
        while (nnz2 < last_in_col2)
        {
            i2 = in_matrix_2->ia[nnz2];
            out_matrix->a[out_matrix->elements_num] = in_matrix_2->a[nnz2];
            out_matrix->ia[out_matrix->elements_num] = i2;
            if (!flag)
            {
                out_matrix->ja[j] = out_matrix->elements_num;
                flag = true;
            }
            out_matrix->elements_num++;
            nnz2++;
        }
        if (!flag)
            out_matrix->ja[j] = -1;
    }
}
