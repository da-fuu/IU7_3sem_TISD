#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "errors.h"
#include "tree.h"
#include "create.h"
#include "analysis.h"
#include "menu.h"


void print_menu(void)
{
    menu_item_t menu_item = EXIT;
    printf("%d: Выйти из программы\n", menu_item++);
    printf("%d: Прочитать дерево из файла\n", menu_item++);
    printf("%d: Перестроить дерево\n", menu_item++);
    printf("%d: Вывести дерево в виде картинки\n", menu_item++);
    printf("%d: Напечатать дерево в префиксном обходе\n", menu_item++);
    printf("%d: Напечатать дерево в инфиксном обходе\n", menu_item++);
    printf("%d: Напечатать дерево в постфиксном обходе\n", menu_item++);
    printf("%d: Добавить элемент в дерево\n", menu_item++);
    printf("%d: Удалить элемент из дерева\n", menu_item++);
    printf("%d: Найти элемент в дереве\n", menu_item++);
    printf("%d: Удалить элементы, обращение к которым было до определенной даты\n", menu_item++);
    printf("%d: Провести сравнение удаления в дереве построенных по разным ключам\n", menu_item);
}

int process_menu(menu_item_t menu_item, tree_t *tree)
{
    FILE *file;
    
    if (tree->root == NULL && menu_item != EXIT && menu_item != RUN_ANALYSIS && menu_item != READ && menu_item != ADD_ELEM)
    {
        puts("Пустое дерево!");
        return OK;
    }

    switch (menu_item)
    {
        case EXIT:
            return NORMAL_EXIT;
        case READ:
            return read_file_from_user(tree);
        case RESORT:
            return tree_change_sort(tree, true);
        case DRAW:
            file = fopen("./graph.gv", "w");
            bin_tree_export_to_dot(file, "Дерево", tree->root);
            fclose(file);
            system("dot -Tpng -o \"out.png\" ./graph.gv");
            system("open out.png");
            return OK;
        case PRINT_PRE:
            bin_tree_print_pre(tree->root);
            return OK;
        case PRINT_IN:
            bin_tree_print_in(tree->root);
            return OK;
        case PRINT_POST:
            bin_tree_print_post(tree->root);
            return OK;
        case ADD_ELEM:
            return insert_from_user(tree);
        case DEL_ELEM:
            return delete_from_user(tree);
        case FIND_ELEM:
            return find_from_user(*tree);
        case DEL_OLD_FILES:
            return del_old_files_from_user(tree);
        case RUN_ANALYSIS:
            return run_analysis();
    }
    return ERR_EOF;
}
