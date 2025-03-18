#include <stdio.h>

#include "tree.h"
#include "errors.h"
#include "menu.h"
#include "input.h"


// Основная функция программы
int main(void)
{
    int rc = OK;
    menu_item_t menu_item;
    tree_t tree;
    tree.root = NULL;
    tree.sorted_by_date = false;

    puts("Программа для работы с деревом");
    while (rc == OK)
    {
        print_menu();
        rc = read_menu_item(&menu_item);
        if (rc != OK)
            break;

        rc = process_menu(menu_item, &tree);
    }
    if (tree.root != NULL)
        bin_tree_free(&tree.root, true);
    if (rc == NORMAL_EXIT)
        rc = OK;
    return rc;
}
