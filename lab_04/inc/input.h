#ifndef INPUT_H__
#define INPUT_H__

#include "struct.h"
#include "menu.h"

#define OK 0
#define ERR_EOF 1
#define NORMAL_EXIT (-1)

void pop_from_user(debug_stack_t *debug_stack);

int push_from_user(debug_stack_t *debug_stack);

int read_menu_item(menu_item_t *menu_item);

int solve_problem_from_user(void);


#endif // #ifndef INPUT_H__
