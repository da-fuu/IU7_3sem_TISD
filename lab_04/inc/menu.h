#ifndef MENU_H__
#define MENU_H__

#include "struct.h"


typedef enum
{
    EXIT = 0,
    INIT_ARRAY_STACK,
    INIT_LIST_STACK,
    PUSH_ELEM,
    POP_ELEM,
    PRINT_STACK,
    PRINT_FREED_MEMORY,
    SOLVE_PROBLEM,
    RUN_ANALYSIS
} menu_item_t;

void print_menu(void);

int process_menu(menu_item_t menu_item, debug_stack_t *debug_stack);


#endif // #ifndef MENU_H__
