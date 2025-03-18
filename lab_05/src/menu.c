#include <stdio.h>

#include "input.h"
#include "array_queue.h"
#include "list_queue.h"
#include "analysis.h"
#include "menu.h"


void print_menu(void)
{
    menu_item_t menu_item = EXIT;
    printf("%d: Выйти из программы\n", menu_item++);
    printf("%d: Инициализировать очередь на массиве\n", menu_item++);
    printf("%d: Инициализировать очередь на списке\n", menu_item++);
    printf("%d: Записать элемент в очередь\n", menu_item++);
    printf("%d: Удалить элемент из очереди\n", menu_item++);
    printf("%d: Напечатать очередь\n", menu_item++);
    printf("%d: Напечатать список освобожденных адресов(только для списка)\n", menu_item++);
    printf("%d: Изменить времена поступления и обработки\n", menu_item++);
    printf("%d: Запустить симуляцию очереди на массиве\n", menu_item++);
    printf("%d: Запустить симуляцию очереди на списке\n", menu_item++);
    printf("%d: Провести сравнение очереди на массиве и списке\n", menu_item);
}

int process_menu(menu_item_t menu_item, params_t *params, debug_queue_t *debug_queue)
{
    abstract_queue_t test_queue_1, test_queue_2;
    int rc;
    
    if (debug_queue->queue == NULL && (menu_item == PUSH_ELEM || menu_item == POP_ELEM || menu_item == PRINT_QUEUE || menu_item == PRINT_FREED_MEMORY))
    {
        puts("Нельзя производить данное действие над неинициализированной очередью!");
        return OK;
    }
    
    if (menu_item == RUN_SIMULATION_ARRAY || menu_item == RUN_SIMULATION_LIST || menu_item == RUN_ANALYSIS)
    {
        if (menu_item != RUN_ANALYSIS)
        {
            test_queue_1.queue = NULL;
            test_queue_2.queue = NULL;
        }
        if (debug_queue->queue != NULL)
        {
            debug_queue->free(debug_queue->queue, debug_queue->freed_memory);
            debug_queue->queue = NULL;
        }
    }
    
    switch (menu_item)
    {
        case EXIT:
            return NORMAL_EXIT;
        case INIT_ARRAY_QUEUE:
            init_array_debug(debug_queue);
            puts("Очередь на массиве успешно инициализирована");
            return OK;
        case INIT_LIST_QUEUE:
            init_list_debug(debug_queue);
            puts("Очередь на списке успешно инициализирована");
            return OK;
        case PUSH_ELEM:
            return push_from_user(debug_queue);
        case POP_ELEM:
            pop_from_user(debug_queue);
            return OK;
        case PRINT_QUEUE:
            if (!debug_queue->print(debug_queue->queue))
                puts("Очередь пуста!");
            return OK;
        case PRINT_FREED_MEMORY:
            debug_queue->print_freed(debug_queue->freed_memory);
            return OK;
        case CHANGE_PARAMS:
            return change_params(params);
        case RUN_SIMULATION_ARRAY:
            init_array(&test_queue_1);
            init_array(&test_queue_2);
            rc = run_simulation(params, &test_queue_1, &test_queue_2, true, NULL);
            if (test_queue_1.queue != NULL)
                test_queue_1.free(test_queue_1.queue); 
            if (test_queue_2.queue != NULL)
                test_queue_2.free(test_queue_2.queue);
            return rc;
        case RUN_SIMULATION_LIST:
            init_list(&test_queue_1);
            init_list(&test_queue_2);
            rc = run_simulation(params, &test_queue_1, &test_queue_2, true, NULL);
            if (test_queue_1.queue != NULL)
                test_queue_1.free(test_queue_1.queue);
            if (test_queue_2.queue != NULL)
                test_queue_2.free(test_queue_2.queue);
            return rc;            
        case RUN_ANALYSIS:
            run_analysis(params);
            return OK;
    }
    return ERR_CRITICAL;
}
