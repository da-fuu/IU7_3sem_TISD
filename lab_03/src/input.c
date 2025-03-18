#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "struct.h"
#include "standard_matrix.h"
#include "input.h"

#define BUF_LEN 6


void init_matrix(both_matrix_t matrix)
{
    matrix.standard_matrix->rows = 0;
    matrix.standard_matrix->cols = 0;
    matrix.sparse_matrix->rows = 0;
    matrix.sparse_matrix->cols = 0;
    matrix.sparse_matrix->elements_num = 0;
}

void flush_stdin(void)
{
    int tmp;
    do
    {
        tmp = getchar();
        if (tmp == EOF)
        {
            puts("Вы нажали ctrl+d или передали на ввод некорректный файл, не надо так!");
            exit(1);
        }
    }
    while (tmp != '\n');
}

static FILE *input_filename(void)
{
    char filename[FILENAME_LEN + 1];
    FILE *f = NULL;
    size_t new_len;

    while (f == NULL)
    {
        if (fgets(filename, FILENAME_LEN + 1, stdin) == NULL)
        {
            puts("Введите непустое имя файла!");
            continue;
        }
        new_len = strcspn(filename, "\n");
        if (new_len < 1)
        {
            puts("Введите непустое имя файла!");
            continue;
        }

        if (filename[new_len] != '\n' && getchar() != '\n')
        {
            flush_stdin();
            printf("Введите имя файла не длиннее %d символов!\n", FILENAME_LEN);
            continue;
        }

        filename[new_len] = '\0';
        f = fopen(filename, "r");
        if (f == NULL)
            puts("Введите корректное имя файла!");
    }
    return f;
}

bool read_num(int *num, int max_val, FILE *file)
{
    char buf[BUF_LEN];
    char *endptr;
    long tmp;

    if (fgets(buf, BUF_LEN, file) == NULL)
        return false;
    if (!isdigit(buf[0]))
        return false;

    tmp = strtol(buf, &endptr, 10);
    if (endptr != strrchr(buf, '\n'))
        return false;
    if (tmp > max_val || tmp <= 0)
        return false;

    *num = (int)tmp;
    return true;
}

static bool read_standard(standard_matrix_t *matrix, bool verbose, FILE *file)
{
    if (verbose)
        printf("Введите значения элементов матрицы размером %d на %d\n", matrix->rows, matrix->cols);

    for (int i = 0; i < matrix->rows; i++)
        for (int j = 0; j < matrix->cols; j++)
            if (fscanf(file, "%d", &matrix->matrix[i][j]) != 1)
            {
                if (verbose)
                    puts("Вы ввели не целые числа!");
                else
                    puts("В файле не только целые числа или слишком мало чисел!");
                return false;
            }
    return true;
}

bool read_dimensions(int *rows, int *cols, bool verbose, FILE *file)
{
    if (verbose)
        puts("Введите количество строк и столбцов:");
    if (fscanf(file, "%d%d", rows, cols) != 2)
    {
        if (verbose)
            puts("Введите 2 числа - количества строк и столбцов!");
        else
            puts("В файле должно быть хотя бы 2 числа - кол-во строк и столбцов!");
        return false;
    }

    if (*rows < 1 || *cols < 1 || *rows > MATRIX_ROWS || *cols > MATRIX_COLS)
    {
        printf("Количество строк должно быть от 1 до %d, а столбцов - от 1 до %d!\n", MATRIX_ROWS, MATRIX_COLS);
        return false;
    }
    return true;
}


void read_file(both_matrix_t matrix_1, both_matrix_t matrix_2)
{
    FILE *file;
    int tmp;
    int rows, cols;

    puts("Введите имя файла с матрицами:");
    file = input_filename();

    if (!read_dimensions(&rows, &cols, false, file))
    {
        fclose(file);
        init_matrix(matrix_1);
        init_matrix(matrix_2);
        return;
    }

    matrix_1.standard_matrix->rows = matrix_2.standard_matrix->rows = rows;
    matrix_1.standard_matrix->cols = matrix_2.standard_matrix->cols = cols;

    if (!read_standard(matrix_1.standard_matrix, false, file))
    {
        fclose(file);
        init_matrix(matrix_1);
        init_matrix(matrix_2);
        return;
    }
    if (!read_standard(matrix_2.standard_matrix, false, file))
    {
        fclose(file);
        init_matrix(matrix_1);
        init_matrix(matrix_2);
        return;
    }
    if (fscanf(file, "%d", &tmp) == 1 || !feof(file))
    {
        puts("В файле больше данных чем требуется!");
        fclose(file);
        init_matrix(matrix_1);
        init_matrix(matrix_2);
        return;
    }
    standard_to_sparse(matrix_1);
    standard_to_sparse(matrix_2);
    puts("Файл прочитан успешно!");
}


void read_user_standard(both_matrix_t matrix_1, both_matrix_t matrix_2)
{
    int rows, cols;

    if (!read_dimensions(&rows, &cols, true, stdin))
    {
        init_matrix(matrix_1);
        init_matrix(matrix_2);
        return;
    }

    matrix_1.standard_matrix->rows = matrix_2.standard_matrix->rows = rows;
    matrix_1.standard_matrix->cols = matrix_2.standard_matrix->cols = cols;

    if (!read_standard(matrix_1.standard_matrix, true, stdin))
    {
        init_matrix(matrix_1);
        init_matrix(matrix_2);
        return;
    }
    if (!read_standard(matrix_2.standard_matrix, true, stdin))
    {
        init_matrix(matrix_1);
        init_matrix(matrix_2);
        return;
    }

    standard_to_sparse(matrix_1);
    standard_to_sparse(matrix_2);
    puts("Матрицы введены успешно");
}

static bool fill_matrix_coord(standard_matrix_t *standard_matrix, int elems_num)
{
    int i, j, val;

    printf("Введите %d троек целых чисел - индекс строки, индекс столбца, значение:\n", elems_num);

    for (int n = 0; n < elems_num; n++)
    {
        if (scanf("%d%d%d", &i, &j, &val) != 3)
        {
            puts("Введены не только целые числа!");
            return false;
        }
        if (i < 0 || i >= standard_matrix->rows || j < 0 || j >= standard_matrix->cols)
        {
            puts("Индексы не могут отрицательными или быть больше соответствующих размерностей!");
            return false;
        }
        standard_matrix->matrix[i][j] = val;
    }
    return true;
}

void read_user_coord(both_matrix_t matrix_1, both_matrix_t matrix_2)
{
    int rows, cols;
    int elements_num;
    int elems_num_1, elems_num_2;

    if (!read_dimensions(&rows, &cols, true, stdin))
    {
        init_matrix(matrix_1);
        init_matrix(matrix_2);
        return;
    }

    matrix_1.standard_matrix->rows = matrix_2.standard_matrix->rows = rows;
    matrix_1.standard_matrix->cols = matrix_2.standard_matrix->cols = cols;
    elements_num = rows * cols;

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < rows; j++)
        {
            matrix_1.standard_matrix->matrix[i][j] = 0;
            matrix_2.standard_matrix->matrix[i][j] = 0;
        }

    printf("Введите количество элементов к вводу в первой матрице(из %d всего):\n", elements_num);
    if (scanf("%d", &elems_num_1) != 1 || elems_num_1 < 0 || elems_num_1 > elements_num)
    {
        printf("Введено количество не от 0 до %d!\n", elements_num);
        init_matrix(matrix_1);
        init_matrix(matrix_2);
        return;
    }

    if (!fill_matrix_coord(matrix_1.standard_matrix, elems_num_1))
    {
        init_matrix(matrix_1);
        init_matrix(matrix_2);
        return;
    }

    printf("Введите количество элементов к вводу во второй матрице(из %d всего):\n", elements_num);
    if (scanf("%d", &elems_num_2) != 1 || elems_num_2 < 0 || elems_num_2 > elements_num)
    {
        printf("Введено количество не от 0 до %d!\n", elements_num);
        init_matrix(matrix_1);
        init_matrix(matrix_2);
        return;
    }
    if (!fill_matrix_coord(matrix_2.standard_matrix, elems_num_2))
    {
        init_matrix(matrix_1);
        init_matrix(matrix_2);
        return;
    }

    standard_to_sparse(matrix_1);
    standard_to_sparse(matrix_2);
    puts("Матрицы введены успешно");
}
