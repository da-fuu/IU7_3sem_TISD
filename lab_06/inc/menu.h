#ifndef MENU_H__
#define MENU_H__

#include "tree.h"


typedef enum
{
    EXIT = 0,
    READ,
    RESORT,
    DRAW,
    PRINT_PRE,
    PRINT_IN,
    PRINT_POST,
    ADD_ELEM,
    DEL_ELEM,
    FIND_ELEM,
    DEL_OLD_FILES,
    RUN_ANALYSIS
} menu_item_t;

void print_menu(void);

int process_menu(menu_item_t menu_item, tree_t *tree);


#endif // #ifndef MENU_H__
