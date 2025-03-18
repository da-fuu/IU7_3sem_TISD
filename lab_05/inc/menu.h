#ifndef MENU_H__
#define MENU_H__

#include "struct.h"
#include "process.h"


typedef enum
{
    EXIT = 0,
    INIT_ARRAY_QUEUE,
    INIT_LIST_QUEUE,
    PUSH_ELEM,
    POP_ELEM,
    PRINT_QUEUE,
    PRINT_FREED_MEMORY,
    CHANGE_PARAMS,
    RUN_SIMULATION_ARRAY,
    RUN_SIMULATION_LIST,
    RUN_ANALYSIS
} menu_item_t;

void print_menu(void);

int process_menu(menu_item_t menu_item, params_t *params, debug_queue_t *debug_queue);


#endif // #ifndef MENU_H__
