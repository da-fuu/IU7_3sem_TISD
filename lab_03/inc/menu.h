#ifndef MENU_H__
#define MENU_H__

#include <stdbool.h>
#include "struct.h"


void print_menu(void);

bool process_menu(menu_item_t menu_item, both_matrix_t in_matrix_1, both_matrix_t in_matrix_2, both_matrix_t out_matrix);


#endif // #ifndef MENU_H__
