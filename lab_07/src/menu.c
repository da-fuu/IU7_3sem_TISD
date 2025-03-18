#include <stdio.h>

#include "input.h"
#include "bst_tree.h"
#include "avl_tree.h"
#include "open_hashtable.h"
#include "closed_hashtable.h"
#include "analysis.h"
#include "menu.h"


void print_menu(void)
{
    menu_item_t menu_item = EXIT;
    printf("%2d: Выйти из программы\n", menu_item++);
    printf("%2d: Инициализировать двоичное дерево поиска\n", menu_item++);
    printf("%2d: Инициализировать AVL-дерево\n", menu_item++);
    printf("%2d: Инициализировать хеш-таблицу с открытым хешированием\n", menu_item++);
    printf("%2d: Инициализировать хеш-таблицу с закрытым хешированием\n", menu_item++);
    printf("%2d: Считать файл в выбранную СД\n", menu_item++);
    printf("%2d: Добавить элемент в выбранную СД\n", menu_item++);
    printf("%2d: Найти элемент в выбранной СД\n", menu_item++);
    printf("%2d: Удалить элемент из выбранной СД\n", menu_item++);
    printf("%2d: Вывести выбранную СД\n", menu_item++);
    printf("%2d: Изменить максимальное количество сравнений\n"
           "    до реструктуризации (только для хеш-таблиц)\n", menu_item++);
    printf("%2d: Провести сравнение эффективности поиска и добавления элементов\n"
           "    в ДДП, AVL-дереве, хеш-таблицах с различным хешированием\n", menu_item);
}

int process_menu(menu_item_t menu_item, assoc_array_t *assoc_array)
{
    if (assoc_array->data == NULL && (menu_item == READ_FILE || menu_item == INSERT_WORD || 
    menu_item == FIND_WORD || menu_item == EXPORT || menu_item == CHANGE_COMPARISONS))
    {
        puts("Нельзя производить данное действие над неинициализированной СД!");
        return OK;
    }

    switch (menu_item)
    {
        case EXIT:
            return NORMAL_EXIT;
        case INIT_BST_TREE:
            init_bst_tree(assoc_array);
            puts("Инициализировано двоичное дерево поиска");
            return OK;     
        case INIT_AVL_TREE:
            init_avl_tree(assoc_array);
            puts("Инициализировано AVL-дерево");
            return OK;    
        case INIT_OPEN_HASHTABLE:
            init_open_hashtable(assoc_array);
            puts("Инициализирована хеш-таблица с открытым хешированием");
            return OK;     
        case INIT_CLOSED_HASHTABLE:
            init_closed_hashtable(assoc_array);
            puts("Инициализирована хеш-таблица с закрытым хешированием");
            return OK;
        case READ_FILE:
            return read_file_from_user(*assoc_array);
        case INSERT_WORD:
            return insert_word_from_user(*assoc_array);
        case FIND_WORD:
            return find_word_from_user(*assoc_array);
        case REMOVE_WORD:
            return remove_word_from_user(*assoc_array);
        case EXPORT:
            return export_from_user(*assoc_array);
        case CHANGE_COMPARISONS:
            return change_comparisons_from_user(*assoc_array);
        // case COMPARE_HT_SIZES:
        //     return compare_ht_sizes();
        case RUN_ANALYSIS:
            return run_analysis();
    }
    return ERR_CRITICAL;
}
