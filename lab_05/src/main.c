#include <stdio.h>

#include "menu.h"
#include "process.h"
#include "input.h"


// Основная функция программы
int main(void)
{
    int rc = OK;
    menu_item_t menu_item;
    debug_queue_t debug_queue;
    params_t params;

    debug_queue.queue = NULL;
    debug_queue.freed_memory = NULL;
    params.t1_lo = 1;
    params.t1_hi = 5;
    params.t2_lo = 0;
    params.t2_hi = 3;
    params.t3_lo = 0;
    params.t3_hi = 4;
    params.t4_lo = 0;
    params.t4_hi = 1;

    puts("Программа для работы с очередью на массиве и списке");
    puts("Для дальнейшей работы инициализируйте очередь на одном из типов данных");
    while (rc == OK)
    {
        print_menu();
        rc = read_menu_item(&menu_item);
        if (rc != OK)
            break;

        rc = process_menu(menu_item, &params, &debug_queue);
    }
    if (debug_queue.queue != NULL)
        debug_queue.free(debug_queue.queue, debug_queue.freed_memory);
    
    if (rc == NORMAL_EXIT)
        rc = OK;
    return rc;
}


