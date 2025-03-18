#include <stdio.h>
#include <stdbool.h>
#include "struct.h"
#include "config.h"
#include "menu.h"
#include "file.h"


// Основная функция программы
int main(void)
{
    bool need_exit = false;
    int menu_num;
    menu_item_t menu_item;
    table_t table;
    table.len = 0;
    key_table_t key_table;

    puts("Программа для работы с таблицей");
    puts("Для дальнейшей работы загрузите таблицу из файла с помощью меню");
    while (!need_exit)
    {
        print_menu();
        do
        {
            printf("Для выбора пункта меню введите целое число от %d до %d\n", EXIT, RUN_ANALYSIS);
        }
        while (!read_num(&menu_num, RUN_ANALYSIS, stdin));

        menu_item = menu_num;
        need_exit = process_menu(menu_item, &table, &key_table);
    }
    return 0;
}
