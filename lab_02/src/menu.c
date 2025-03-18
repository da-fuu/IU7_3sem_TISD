#include <stdio.h>
#include "config.h"
#include "struct.h"
#include "print.h"
#include "key_table.h"
#include "analysis.h"
#include "file.h"
#include "operations.h"
#include "menu.h"


void print_menu(void)
{
    menu_item_t menu_item = EXIT - 1;
    printf("%d: Выйти из программы\n", ++menu_item);
    printf("%d: Считать таблицу из файла\n", ++menu_item);
    printf("%d: Сохранить таблицу в файл\n", ++menu_item);
    printf("%d: Вывести исходную таблицу\n", ++menu_item);
    printf("%d: Вывести исходную таблицу ключей\n", ++menu_item);
    printf("%d: Вывести отсортированную таблицу\n", ++menu_item);
    printf("%d: Вывести отсортированную таблицу ключей\n", ++menu_item);
    printf("%d: Вывести исходную таблицу в отсортированном виде, используя отсортированную таблицу ключей\n", ++menu_item);
    printf("%d: Добавить запись в конец таблицы\n", ++menu_item);
    printf("%d: Удалить запись из таблицы\n", ++menu_item);
    printf("%d: Вывести список всех друзей, которых необходимо поздравить с днем рождения в ближайшую неделю\n", ++menu_item);
    printf("%d: Провести сравнение различных способов и алгоритмов сортировок\n", ++menu_item);
}

bool process_menu(menu_item_t menu_item, table_t *table, key_table_t *key_table)
{
    if (table->len == 0 && menu_item != EXIT && menu_item != READ_TABLE && menu_item != ADD_RECORD && menu_item != RUN_ANALYSIS)
    {
        puts("Нельзя производить данное действие над пустой таблицей!");
        puts("Прочтите таблицу из файла или добавьте записи вручную!");
        return false;
    }

    switch (menu_item)
    {
        case EXIT:
            return true;
        case READ_TABLE:
            read_table(table);
            return false;
        case SAVE_TABLE:
            save_table(table);
            return false;
        case PRINT_TABLE:
            print_table(table);
            return false;
        case PRINT_KEY_TABLE:
            create_print_key_table(table, key_table);
            return false;
        case SORT_TABLE:
            print_sorted_table(table);
            return false;
        case SORT_KEY_TABLE:
            print_sorted_key_table(table, key_table);
            return false;
        case SORT_TABLE_WITH_KEY_TABLE:
            sort_table_with_key_table(table, key_table);
            return false;
        case ADD_RECORD:
            add_record_to_table(table);
            return false;
        case DELETE_RECORD:
            delete_record_from_table(table);
            return false;
        case PRINT_FRIENDS_WITH_NEAR_BDAY:
            print_friends_with_bday(table);
            return false;
        case RUN_ANALYSIS:
            run_analysis();
            return false;
        default:
            puts("Неизвестный пункт меню!!?!");
            return true;
    }
}
