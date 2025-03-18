#include <time.h>
#include <stdio.h>
#include <string.h>
#include "standard_matrix.h"
#include "sparse_matrix.h"
#include "analysis.h"

#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

typedef enum
{
    STANDARD = 0,
    SPARSE
} sum_method_t;


static unsigned long long run_test_sum(void *in_matrix_1, void *in_matrix_2, void *out_matrix, sum_method_t sum_method)
{
    struct timespec beg, end;
    unsigned long long diff_ns;

    switch (sum_method)
    {
        case STANDARD:
            clock_gettime(CLOCK_MONOTONIC_RAW, &beg);
            sum_standard(in_matrix_1, in_matrix_2, out_matrix);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            break;
        case SPARSE:
            clock_gettime(CLOCK_MONOTONIC_RAW, &beg);
            sum_sparse(in_matrix_1, in_matrix_2, out_matrix);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            break;
    }
    diff_ns = (unsigned long long)(end.tv_sec - beg.tv_sec) * 1000 * 1000 * 1000 + (end.tv_nsec - beg.tv_nsec);
    return diff_ns;
}

static unsigned long long run_tests(void *in_matrix_1, void *in_matrix_2, void *out_matrix, sum_method_t sum_method)
{
    unsigned long long res = 0;

    for (int i = 0; i < START_GARBAGE_NUM; i++)
        run_test_sum(in_matrix_1, in_matrix_2, out_matrix, sum_method);

    for (int i = 0; i < TESTS_NUM; i++)
        res += run_test_sum(in_matrix_1, in_matrix_2, out_matrix, sum_method);

    res /= TESTS_NUM;
    return res;
}

static double calc_gain(unsigned long long before, unsigned long long after)
{
    return (1.0 - (double)after / (double)before) * 100.0;
}

void run_analysis(both_matrix_t in_matrix_1, both_matrix_t in_matrix_2, both_matrix_t out_matrix)
{
    int sizes[][2] = { { 10, 10 }, { 100, 100 }, { 1000, 1000 } };

    unsigned long long results[ARR_LEN(sizes)][101][2];
    double gains[ARR_LEN(sizes)][101];


    puts("Результаты сравнения (в наносекундах):");
    for (size_t i = 0; i < ARR_LEN(sizes); i++)
    {
        printf("\nРазмеры матриц: %d x %d:\n", sizes[i][0], sizes[i][1]);
        printf("| Процент заполнения | Время сложения стандартных матриц | Время сложения разреженных матриц | Выигрыш времени при сложении разреженных |\n");

        in_matrix_1.standard_matrix->rows = sizes[i][0];
        in_matrix_1.standard_matrix->cols = sizes[i][1];
        in_matrix_2.standard_matrix->rows = sizes[i][0];
        in_matrix_2.standard_matrix->cols = sizes[i][1];

        for (int percent = 0; percent <= 100; percent++)
        {
            fill_standard(in_matrix_1.standard_matrix, sizes[i][0] * sizes[i][1] * percent / 100);
            fill_standard(in_matrix_2.standard_matrix, sizes[i][0] * sizes[i][1] * percent / 100);

            standard_to_sparse(in_matrix_1);
            standard_to_sparse(in_matrix_2);

            results[i][percent][STANDARD] = run_tests(in_matrix_1.standard_matrix, in_matrix_2.standard_matrix, out_matrix.standard_matrix, STANDARD);
            results[i][percent][SPARSE] = run_tests(in_matrix_1.sparse_matrix, in_matrix_2.sparse_matrix, out_matrix.sparse_matrix, SPARSE);
            gains[i][percent] = calc_gain(results[i][percent][STANDARD], results[i][percent][SPARSE]);
            printf("| %18d | %33llu | %33llu | %39lf%% |\n", percent, results[i][percent][STANDARD], results[i][percent][SPARSE], gains[i][percent]);
        }
    }
}
