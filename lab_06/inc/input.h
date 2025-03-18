#ifndef INPUT_H__
#define INPUT_H__

#include "tree.h"
#include "menu.h"


int insert_from_user(tree_t *tree);

int find_from_user(tree_t tree);

int delete_from_user(tree_t *tree);

int read_menu_item(menu_item_t *menu_item);

int read_file_from_user(tree_t *tree);

int del_old_files_from_user(tree_t *tree);

int read_file(tree_t *tree, char *filename);


#endif // #ifndef INPUT_H__
