#include <stdio.h>

#include "input.h"
#include "array_stack.h"
#include "list_stack.h"
#include "analysis.h"
#include "menu.h"


void print_menu(void)
{
    menu_item_t menu_item = EXIT;
    printf("%d: Выйти из программы\n", menu_item++);
    printf("%d: Инициализировать стек на массиве\n", menu_item++);
    printf("%d: Инициализировать стек на списке\n", menu_item++);
    printf("%d: Записать элемент на стек\n", menu_item++);
    printf("%d: Снять элемент со стека\n", menu_item++);
    printf("%d: Напечатать стек\n", menu_item++);
    printf("%d: Напечатать массив освобожденных адресов(только для списка)\n", menu_item++);
    printf("%d: Найти значение выражения\n", menu_item++);
    printf("%d: Провести сравнение стека на массиве и списке\n", menu_item);
}

int process_menu(menu_item_t menu_item, debug_stack_t *debug_stack)
{
    if (debug_stack->stack == NULL && menu_item != EXIT && menu_item != RUN_ANALYSIS && menu_item != SOLVE_PROBLEM
        && menu_item != INIT_LIST_STACK && menu_item != INIT_ARRAY_STACK)
    {
        puts("Нельзя производить данное действие над неинициализированным стеком!");
        return OK;
    }

    switch (menu_item)
    {
        case EXIT:
            return NORMAL_EXIT;
        case INIT_ARRAY_STACK:
            init_array_debug(debug_stack);
            puts("Стек на массиве успешно инициализирован");
            return OK;
        case INIT_LIST_STACK:
            init_list_debug(debug_stack);
            puts("Стек на списке успешно инициализирован");
            return OK;
        case PUSH_ELEM:
            return push_from_user(debug_stack);
        case POP_ELEM:
            pop_from_user(debug_stack);
            return OK;
        case PRINT_STACK:
            if (!debug_stack->print(debug_stack->stack))
                puts("Стек пуст!");
            return OK;
        case PRINT_FREED_MEMORY:
            debug_stack->print_freed(debug_stack->freed_memory);
            return OK;
        case SOLVE_PROBLEM:
            return solve_problem_from_user();
        case RUN_ANALYSIS:
            run_analysis();
            return OK;
    }
    return ERR_EOF;
}
