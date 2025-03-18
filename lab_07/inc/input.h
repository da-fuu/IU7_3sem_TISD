#ifndef INPUT_H__
#define INPUT_H__

#include "struct.h"
#include "menu.h"

#define OK 0
#define ERR_CRITICAL 1
#define NORMAL_EXIT (-1)


int read_file_from_user(assoc_array_t assoc_array);

int insert_word_from_user(assoc_array_t assoc_array);

int find_word_from_user(assoc_array_t assoc_array);

int remove_word_from_user(assoc_array_t assoc_array);

int export_from_user(assoc_array_t assoc_array);

int change_comparisons_from_user(assoc_array_t assoc_array);

int read_menu_item(menu_item_t *menu_item);


bool read_words(assoc_array_t assoc_array, FILE *file);

#endif // #ifndef INPUT_H__
