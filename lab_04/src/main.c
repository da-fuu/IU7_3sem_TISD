#include <stdio.h>

#include "struct.h"
#include "menu.h"
#include "input.h"


// Основная функция программы
int main(void)
{
    int rc = OK;
    menu_item_t menu_item;
    debug_stack_t debug_stack;
    debug_stack.stack = NULL;
    debug_stack.freed_memory = NULL;

    puts("Программа для работы со стеком на массиве и списке");
    puts("Для дальнейшей работы инициализируйте стек на одном из типов данных");
    while (rc == OK)
    {
        print_menu();
        rc = read_menu_item(&menu_item);
        if (rc != OK)
            break;

        rc = process_menu(menu_item, &debug_stack);
    }
    if (debug_stack.stack != NULL)
        debug_stack.free(debug_stack.stack, debug_stack.freed_memory);
    if (rc == NORMAL_EXIT)
        rc = OK;
    return rc;
}
