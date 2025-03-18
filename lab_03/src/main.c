#include <stdio.h>
#include <stdbool.h>
#include "struct.h"
#include "menu.h"
#include "input.h"


// Основная функция программы
int main(void)
{
    bool need_exit = false;
    int menu_num;
    menu_item_t menu_item;
    static standard_matrix_t standard_matrix_in1, standard_matrix_in2, standard_matrix_out;
    static sparse_matrix_t sparse_matrix_in1, sparse_matrix_in2, sparse_matrix_out;

    both_matrix_t in_matrix_1 = { .standard_matrix = &standard_matrix_in1, .sparse_matrix = &sparse_matrix_in1 };
    both_matrix_t in_matrix_2 = { .standard_matrix = &standard_matrix_in2, .sparse_matrix = &sparse_matrix_in2 };
    both_matrix_t out_matrix = { .standard_matrix = &standard_matrix_out, .sparse_matrix = &sparse_matrix_out };
    init_matrix(in_matrix_1);
    init_matrix(in_matrix_2);
    init_matrix(out_matrix);

    puts("Программа для работы с матрицами в стандартном и разреженном представлении");
    puts("Для дальнейшей работы загрузите матрицы из файла или заполните вручную с помощью меню");
    while (!need_exit)
    {
        print_menu();
        do
        {
            printf("Для выбора пункта меню введите целое число от %d до %d\n", EXIT, RUN_ANALYSIS);
        }
        while (!read_num(&menu_num, RUN_ANALYSIS, stdin));

        menu_item = menu_num;
        need_exit = process_menu(menu_item, in_matrix_1, in_matrix_2, out_matrix);
    }
    return 0;
}
