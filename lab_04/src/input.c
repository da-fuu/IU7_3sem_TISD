#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "process.h"
#include "menu.h"
#include "print.h"
#include "input.h"


void pop_from_user(debug_stack_t *debug_stack)
{
    token_t token;
    if (!debug_stack->pop_mem(debug_stack->stack, &token, debug_stack->freed_memory))
    {
        puts("Стек пуст!");
        return;
    }
    puts("Бывшее значение на вершине стека:");
    print_token(&token);
}

int push_from_user(debug_stack_t *debug_stack)
{
    char *buf = NULL;
    size_t len = 0;
    char *endptr;
    token_t token;
    char chr_start, chr_end;

    puts("Введите элемент стека - вещественное число в формате IEEE754 или символ действия [+-*/]:");
    if (getline(&buf, &len, stdin) == -1)
    {
        free(buf);
        return ERR_EOF;
    }
    len = strlen(buf);
    if (len < 2)
    {
        free(buf);
        puts("Введено пустое значение!");
        return OK;
    }

    token.value.operand = strtod(buf, &endptr);
    chr_start = buf[0];
    chr_end = *endptr;
    free(buf);

    if (chr_end == '\n')
    {
        token.is_operator = false;
        if (debug_stack->push(debug_stack->stack, token))
            puts("В стек успешно добавлено число");
        else
            puts("Стек переполнен!");
        return OK;
    }
    if (len > 2)
    {
        puts("Введено некорректное значение!");
        return OK;
    }
    if (is_operator(chr_start))
    {
        token.is_operator = true;
        token.value.operator = chr_start;
        if (debug_stack->push(debug_stack->stack, token))
            puts("В стек успешно добавлен оператор");
        else
            puts("Стек переполнен!");
        return OK;
    }
    puts("Введено некорректное значение!");
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
            return ERR_EOF;
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


int solve_problem_from_user(void)
{
    char *buf = NULL;
    size_t n = 0;
    double out;
    bool ret_flag;

    puts("Введите выражение в виде (число)(знак)(число)...(знак)(число):");
    if (getline(&buf, &n, stdin) == -1)
    {
        free(buf);
        return ERR_EOF;
    }
    n = strlen(buf);
    if (n < 2 || n > 4094 || buf[n - 1] != '\n')
    {
        free(buf);
        puts("Введено некорректное выражение!");
        return OK;
    }
    buf[--n] = '\0';

    ret_flag = solve_problem_from_str(buf, &out);
    free(buf);
    if (ret_flag)
    {
        if (!isinf(out) && !isnan(out))
            printf("Значение выражения: %lf\n", out);
        else
            puts("Деление на 0 в выражении!");
    }
    else
        puts("Введено некорректное выражение!");

    return OK;
}