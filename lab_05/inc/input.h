#ifndef INPUT_H__
#define INPUT_H__

#include "struct.h"
#include "process.h"
#include "menu.h"

#define OK 0
#define ERR_CRITICAL 1
#define NORMAL_EXIT (-1)


void pop_from_user(debug_queue_t *debug_queue);

int push_from_user(debug_queue_t *debug_queue);

int read_menu_item(menu_item_t *menu_item);

int change_params(params_t *params);


#endif // #ifndef INPUT_H__
