#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "menu.h"
#include "process.h"
#include "input.h"


#define READ_FLOAT(address) \
do {                                 \
    int rc = read_double(address); \
    if (rc != OK)                       \
    {                       \
        if (rc == NORMAL_EXIT)  \
            return OK;              \
        return rc;                \
    }                         \
}                         \
while (0)

#define READ_T(p_lo, p_hi) \
do                         \
{                       \
    double lo, hi;      \
    READ_FLOAT(&lo); \
    READ_FLOAT(&hi); \
    if (lo > hi) \
    { \
        puts("Введены некорректные значения!"); \
        return OK; \
    } \
    (p_lo) = lo; \
    (p_hi) = hi; \
} \
while (0)


void pop_from_user(debug_queue_t *debug_queue)
{
    value_t value;
    if (!debug_queue->pop_mem(debug_queue->queue, &value, debug_queue->freed_memory))
    {
        puts("Очередь пуста!");
        return;
    }
    puts("Бывшее значение в начале очереди:");
    printf("%lf\n", value.time);
}

int read_double(double *out)
{
    char *buf = NULL;
    size_t len;
    char *endptr;
    
    if (getline(&buf, &len, stdin) == -1)
    {
        free(buf);
        return ERR_CRITICAL;
    }
    len = strlen(buf);
    if (len < 2)
    {
        free(buf);
        puts("Введено пустое значение!");
        return NORMAL_EXIT;
    }

    *out = strtod(buf, &endptr);

    if (*endptr != '\n')
    {
        free(buf);
        puts("Введено некорректное значение!");
        return NORMAL_EXIT;
    }
    free(buf);
    
    if (*out < 0.0)
    {
        puts("Введено некорректное значение!");
        return NORMAL_EXIT;
    }
    
    return OK;
}


int push_from_user(debug_queue_t *debug_queue)
{
    value_t value;

    puts("Введите элемент очереди - вещественное число в формате IEEE754:");
    
    READ_FLOAT(&value.time);
    
    if (debug_queue->push(debug_queue->queue, value))
        puts("В очередь успешно добавлено число");
    else
        puts("Очередь переполнена!");
    return OK;
}

int read_menu_item(menu_item_t *menu_item)
{
    char *buf = NULL;
    size_t n = 0;
    char *endptr;
    long tmp;
    bool flag = false;

    do
    {
        printf("Для выбора пункта меню введите целое число от %d до %d\n", EXIT, RUN_ANALYSIS);
        if (getline(&buf, &n, stdin) == -1)
        {
            free(buf);
            return ERR_CRITICAL;
        }

        if (!isdigit(buf[0]))
            continue;

        tmp = strtol(buf, &endptr, 10);
        if (*endptr != '\n')
            continue;
        if (tmp > RUN_ANALYSIS || tmp < 0)
            continue;
        flag = true;
    }
    while (!flag);
    *menu_item = tmp;
    free(buf);

    return OK;
}

int change_params(params_t *params)
{
    puts("Введите 8 вещественных чисел в формате IEEE754, каждое в отдельной строке - нижние и верхние пределы времен T1, T2, T3, T4:");
    
    READ_T(params->t1_lo, params->t1_hi);
    READ_T(params->t2_lo, params->t2_hi);
    READ_T(params->t3_lo, params->t3_hi);
    READ_T(params->t4_lo, params->t4_hi);
    
    if (expected_time(params->t4_lo, params->t4_hi) > expected_time(params->t2_lo, params->t2_hi))
        puts("Warning: Заявки во второй очереди будут поступать быстрее чем обрабатываться!\nБольшой шанс вечной работы программы!");
    
    return OK;
}
