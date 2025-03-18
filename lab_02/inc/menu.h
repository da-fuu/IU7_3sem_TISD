#ifndef MENU_H__
#define MENU_H__

#include <stdbool.h>
#include "struct.h"


void print_menu(void);

bool process_menu(menu_item_t menu_item, table_t *table, key_table_t *key_table);


#endif // #ifndef MENU_H__
