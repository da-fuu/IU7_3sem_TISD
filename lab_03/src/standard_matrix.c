#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "print.h"
#include "input.h"
#include "standard_matrix.h"


void standard_to_sparse(both_matrix_t matrix)
{
    bool has_elem_in_col;
    matrix.sparse_matrix->elements_num = 0;

    matrix.sparse_matrix->rows = matrix.standard_matrix->rows;
    matrix.sparse_matrix->cols = matrix.standard_matrix->cols;

    for (int j = 0; j < matrix.standard_matrix->cols; j++)
    {
        has_elem_in_col = false;
        for (int i = 0; i < matrix.standard_matrix->rows; i++)
            if (matrix.standard_matrix->matrix[i][j] != 0)
            {
                if (!has_elem_in_col)
                {
                    has_elem_in_col = true;
                    matrix.sparse_matrix->ja[j] = matrix.sparse_matrix->elements_num;
                }
                matrix.sparse_matrix->a[matrix.sparse_matrix->elements_num] = matrix.standard_matrix->matrix[i][j];
                matrix.sparse_matrix->ia[matrix.sparse_matrix->elements_num] = i;
                matrix.sparse_matrix->elements_num++;
            }
        if (!has_elem_in_col)
            matrix.sparse_matrix->ja[j] = -1;
    }
}

void fill_standard(standard_matrix_t *standard_matrix, int nonzero_elems)
{
    int j;
    int tmp;
    int cols = standard_matrix->cols;
    int elements_num = standard_matrix->rows * standard_matrix->cols;

    for (int i = 0; i < nonzero_elems; i++)
        standard_matrix->matrix[i / cols][i % cols] = i + 1;
    for (int i = nonzero_elems; i < elements_num; i++)
        standard_matrix->matrix[i / cols][i % cols] = 0;

    for (int i = elements_num - 1; i > 0; i--)
    {
        j = rand() % (i + 1);
        tmp = standard_matrix->matrix[i / cols][i % cols];
        standard_matrix->matrix[i / cols][i % cols] = standard_matrix->matrix[j / cols][j % cols];
        standard_matrix->matrix[j / cols][j % cols] = tmp;
    }
}

void fill_with_sparsity(both_matrix_t matrix_1, both_matrix_t matrix_2)
{
    int rows, cols;
    int nonzero_el_1, nonzero_el_2;
    int elements_num;

    if (!read_dimensions(&rows, &cols, true, stdin))
    {
        init_matrix(matrix_1);
        init_matrix(matrix_2);
        return;
    }
    elements_num = rows * cols;

    printf("Введите количество ненулевых элементов в первой матрице(из %d всего):\n", elements_num);
    if (scanf("%d", &nonzero_el_1) != 1 || nonzero_el_1 < 0 || nonzero_el_1 > elements_num)
    {
        printf("Введено количество не от 0 до %d!\n", elements_num);
        init_matrix(matrix_1);
        init_matrix(matrix_2);
        return;
    }

    printf("Введите количество ненулевых элементов во второй матрице(из %d всего):\n", elements_num);
    if (scanf("%d", &nonzero_el_2) != 1 || nonzero_el_2 < 0 || nonzero_el_2 > elements_num)
    {
        printf("Введено количество не от 0 до %d!\n", elements_num);
        init_matrix(matrix_1);
        init_matrix(matrix_2);
        return;
    }
    matrix_1.standard_matrix->rows = matrix_2.standard_matrix->rows = rows;
    matrix_1.standard_matrix->cols = matrix_2.standard_matrix->cols = cols;

    fill_standard(matrix_1.standard_matrix, nonzero_el_1);
    fill_standard(matrix_2.standard_matrix, nonzero_el_2);

    standard_to_sparse(matrix_1);
    standard_to_sparse(matrix_2);
    puts("Матрицы заполнены успешно");
}

void sum_standard(const standard_matrix_t *in_matrix_1, const standard_matrix_t *in_matrix_2, standard_matrix_t *out_matrix)
{
    out_matrix->rows = in_matrix_1->rows;
    out_matrix->cols = in_matrix_1->cols;

    for (int i = 0; i < out_matrix->rows; i++)
        for (int j = 0; j < out_matrix->cols; j++)
            out_matrix->matrix[i][j] = in_matrix_1->matrix[i][j] + in_matrix_2->matrix[i][j];
}

void fill_standard_with_zeroes(standard_matrix_t *matrix)
{
    for (int i = 0; i < matrix->rows; i++)
        for (int j = 0; j < matrix->cols; j++)
            matrix->matrix[i][j] = 0;
}
