#include <stdio.h>

#include "struct.h"
#include "menu.h"
#include "input.h"


// Основная функция программы
int main(void)
{
    int rc = OK;
    menu_item_t menu_item;
    assoc_array_t assoc_array;
    assoc_array.data = NULL;

    puts("Программа для работы с деревьями поиска и хеш-таблицами");
    puts("Для дальнейшей работы инициализируйте одну из СД");
    while (rc == OK)
    {
        print_menu();
        rc = read_menu_item(&menu_item);
        if (rc != OK)
            break;

        rc = process_menu(menu_item, &assoc_array);
    }
    if (assoc_array.data != NULL)
        assoc_array.free(&assoc_array.data);
    if (rc == NORMAL_EXIT)
        rc = OK;
    return rc;
}
