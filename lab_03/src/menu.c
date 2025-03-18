#include <stdio.h>
#include "struct.h"
#include "print.h"
#include "analysis.h"
#include "standard_matrix.h"
#include "sparse_matrix.h"
#include "input.h"
#include "menu.h"


void print_menu(void)
{
    menu_item_t menu_item = EXIT - 1;
    printf("%d: Выйти из программы\n", ++menu_item);
    printf("%d: Считать матрицы из файла\n", ++menu_item);
    printf("%d: Ввести матрицы в стандартном виде\n", ++menu_item);
    printf("%d: Ввести матрицы координатным способом\n", ++menu_item);
    printf("%d: Заполнить матрицы с заданной заполненностью\n", ++menu_item);
    printf("%d: Вывести матрицы в стандартном виде\n", ++menu_item);
    printf("%d: Вывести матрицы в разреженном виде\n", ++menu_item);
    printf("%d: Найти сумму стандартных матриц и вывести в стандартном виде\n", ++menu_item);
    printf("%d: Найти сумму стандартных матриц и вывести в разреженном виде\n", ++menu_item);
    printf("%d: Найти сумму разреженных матриц и вывести в стандартном виде\n", ++menu_item);
    printf("%d: Найти сумму разреженных матриц и вывести в разреженном виде\n", ++menu_item);
    printf("%d: Провести сравнение различных способов умножения матриц\n", ++menu_item);
}

bool process_menu(menu_item_t menu_item, both_matrix_t in_matrix_1, both_matrix_t in_matrix_2, both_matrix_t out_matrix)
{
    if ((in_matrix_1.standard_matrix->cols == 0 || in_matrix_1.standard_matrix->rows == 0 ||
        in_matrix_2.standard_matrix->cols == 0 || in_matrix_2.standard_matrix->rows == 0) &&
        (menu_item != EXIT && menu_item != READ_FILE && menu_item != READ_USER_STANDARD &&
        menu_item != READ_USER_COORD && menu_item != FILL_WITH_SPARSITY && menu_item != RUN_ANALYSIS))
    {
        puts("Нельзя производить данное действие над пустыми матрицами!");
        puts("Прочтите матрицы из файла или заполните вручную!");
        return false;
    }

    switch (menu_item)
    {
        case EXIT:
            return true;
        case READ_FILE:
            read_file(in_matrix_1, in_matrix_2);
            return false;
        case READ_USER_STANDARD:
            read_user_standard(in_matrix_1, in_matrix_2);
            flush_stdin();
            return false;
        case READ_USER_COORD:
            read_user_coord(in_matrix_1, in_matrix_2);
            flush_stdin();
            return false;
        case FILL_WITH_SPARSITY:
            fill_with_sparsity(in_matrix_1, in_matrix_2);
            flush_stdin();
            return false;
        case PRINT_STANDARD:
            puts("Первая матрица в стандартном виде:");
            print_standard(in_matrix_1.standard_matrix);
            puts("Вторая матрица в стандартном виде:");
            print_standard(in_matrix_2.standard_matrix);
            return false;
        case PRINT_SPARSE:
            puts("Первая матрица в разреженном виде:");
            print_sparse(in_matrix_1.sparse_matrix);
            puts("Вторая матрица в разреженном виде:");
            print_sparse(in_matrix_2.sparse_matrix);
            return false;
        case SUM_STANDARD_OUT_ST:
            sum_standard(in_matrix_1.standard_matrix, in_matrix_2.standard_matrix, out_matrix.standard_matrix);
            puts("Получившаяся матрица в стандартном виде::");
            print_standard(out_matrix.standard_matrix);
            return false;
        case SUM_STANDARD_OUT_SP:
            sum_standard(in_matrix_1.standard_matrix, in_matrix_2.standard_matrix, out_matrix.standard_matrix);
            puts("Получившаяся матрица в разреженном виде:");
            standard_to_sparse(out_matrix);
            print_sparse(out_matrix.sparse_matrix);
            return false;
        case SUM_SPARSE_OUT_ST:
            sum_sparse(in_matrix_1.sparse_matrix, in_matrix_2.sparse_matrix, out_matrix.sparse_matrix);
            sparse_to_standard(out_matrix);
            puts("Получившаяся матрица в стандартном виде:");
            print_standard(out_matrix.standard_matrix);
            return false;
        case SUM_SPARSE_OUT_SP:
            sum_sparse(in_matrix_1.sparse_matrix, in_matrix_2.sparse_matrix, out_matrix.sparse_matrix);
            puts("Получившаяся матрица в разреженном виде:");
            print_sparse(out_matrix.sparse_matrix);
            return false;
        case RUN_ANALYSIS:
            run_analysis(in_matrix_1, in_matrix_2, out_matrix);
            return false;
        default:
            return true;
    }
}
