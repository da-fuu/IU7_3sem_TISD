#ifndef MENU_H__
#define MENU_H__

#include "struct.h"


typedef enum
{
    EXIT = 0,
    INIT_BST_TREE,
    INIT_AVL_TREE,
    INIT_OPEN_HASHTABLE,
    INIT_CLOSED_HASHTABLE,
    READ_FILE,
    INSERT_WORD,
    FIND_WORD,
    REMOVE_WORD,
    EXPORT,
    CHANGE_COMPARISONS,
    RUN_ANALYSIS
} menu_item_t;

void print_menu(void);

int process_menu(menu_item_t menu_item, assoc_array_t *assoc_array);


#endif // #ifndef MENU_H__
