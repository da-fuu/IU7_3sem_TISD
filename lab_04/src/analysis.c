#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "array_stack.h"
#include "list_stack.h"
#include "process.h"
#include "analysis.h"

#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))


static unsigned long long run_one_test(stack_t *stack_1, stack_t *stack_2, token_t input[], int len, void (*init)(stack_t *))
{
    struct timespec beg, end;
    unsigned long long diff_ns;

    init(stack_1);
    init(stack_2);
    for (int i = 0; i < len; i++)
    {
        stack_1->push(stack_1->stack, input[i]);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &beg);
    solve_problem(stack_1, stack_2);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    diff_ns = (unsigned long long)(end.tv_sec - beg.tv_sec) * 1000 * 1000 * 1000 + (end.tv_nsec - beg.tv_nsec);
    return diff_ns;
}

static unsigned long long int run_tests(stack_t *stack_1, stack_t *stack_2, token_t input[], int len, void (*init)(stack_t *), int n_tests)
{
    unsigned long long res = 0;

    for (int i = 0; i < START_GARBAGE_NUM; i++)
        run_one_test(stack_1, stack_2, input, len, init);

    for (int i = 0; i < n_tests; i++)
        res += run_one_test(stack_1, stack_2, input, len, init);

    res /= n_tests;
    return res;
}

static double calc_gain(unsigned long long before, unsigned long long after)
{
    return (1.0 - (double)after / (double)before) * 100.0;
}

void run_analysis()
{
    int sizes[] = { 2, 25, 250, 2500, 25000 };
    stack_t stack_1;
    stack_t stack_2;
    token_t input_arr[STACK_DEPTH];
    char ops[4] = { '+', '-', '*', '/' };
    size_t arr_size = sizeof(array_stack_t);
    size_t list_size;
    stack_1.stack = NULL;
    stack_2.stack = NULL;
    srand(sizes[ARR_LEN(sizes) - 1]);

    unsigned long long results[2];
    double gain;

    for (size_t i = 0; i < STACK_DEPTH; i++)
        if (i % 2 == 0)
        {
            input_arr[i].is_operator = false;
            input_arr[i].value.operand = (double)rand() / (float)(RAND_MAX / 1000.0) - 500.0;
        }
        else
        {
            input_arr[i].is_operator = true;
            input_arr[i].value.operator = ops[rand() % 4];
        }

    puts("Результаты сравнения (в наносекундах):");
    printf("| Длина выражения | Время расчета списками | Время расчета массивами | Выигрыш времени от стека на массиве | Размер стека на списке, байт | Выигрыш памяти от стека на списке |\n");

    for (size_t i = 0; i < ARR_LEN(sizes); i++)
    {
        sizes[i] = sizes[i] * 2 + 1;

        results[0] = run_tests(&stack_1, &stack_2, input_arr, sizes[i], init_array, TESTS_MUL / sizes[i]);
        results[1] = run_tests(&stack_1, &stack_2, input_arr, sizes[i], init_list, TESTS_MUL / sizes[i]);
        gain = calc_gain(results[1], results[0]);
        list_size = sizeof(list_node_t) * sizes[i] + sizeof(list_stack_t);
        printf("| %15d | %22llu | %23llu | %34.2lf%% | %28zu | %32.2lf%% |\n", sizes[i], results[1], results[0], gain, list_size, calc_gain(arr_size, list_size));
    }

    printf("Размер стека на массиве, байт: %zu\n", arr_size);
    stack_1.free(stack_1.stack);
    stack_2.free(stack_2.stack);
}
