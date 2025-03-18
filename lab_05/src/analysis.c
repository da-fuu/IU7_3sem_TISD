#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "array_queue.h"
#include "list_queue.h"
#include "input.h"
#include "analysis.h"


static unsigned long long run_one_test(abstract_queue_t *queue_1, abstract_queue_t *queue_2, params_t *params, void (*init)(abstract_queue_t *), int *max_queue_len)
{
    struct timespec beg, end;
    unsigned long long diff_ns;

    init(queue_1);
    init(queue_2);

    clock_gettime(CLOCK_MONOTONIC_RAW, &beg);
    if (run_simulation(params, queue_1, queue_2, false, max_queue_len) != OK)
    {
        queue_1->free(queue_1->queue);
        queue_2->free(queue_2->queue);
        exit(ERR_CRITICAL);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    diff_ns = (unsigned long long)(end.tv_sec - beg.tv_sec) * 1000 * 1000 * 1000 + (end.tv_nsec - beg.tv_nsec);
    return diff_ns;
}

static unsigned long long run_tests(abstract_queue_t *queue_1, abstract_queue_t *queue_2, params_t *params, void (*init)(abstract_queue_t *), int *max_queue_len)
{
    unsigned long long res = 0;

    for (int i = 0; i < START_GARBAGE_NUM; i++)
        run_one_test(queue_1, queue_2, params, init, max_queue_len);

    for (int i = 0; i < TESTS; i++)
        res += run_one_test(queue_1, queue_2, params, init, max_queue_len);

    res /= TESTS;
    return res;
}


static unsigned long long run_push(abstract_queue_t *queue, void (*init)(abstract_queue_t *))
{
    init(queue);
    value_t value;
    struct timespec beg, end;
    unsigned long long diff_ns;

    clock_gettime(CLOCK_MONOTONIC_RAW, &beg);
    for (int i = 0; i < QUEUE_DEPTH; i++)
    {
        value.time = i;
        queue->push(queue->queue, value);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    diff_ns = (unsigned long long)(end.tv_sec - beg.tv_sec) * 1000 * 1000 * 1000 + (end.tv_nsec - beg.tv_nsec);
    return diff_ns / QUEUE_DEPTH;
}

static unsigned long long run_pop(abstract_queue_t *queue)
{
    value_t value;
    struct timespec beg, end;
    unsigned long long diff_ns;

    clock_gettime(CLOCK_MONOTONIC_RAW, &beg);
    for (int i = 0; i < QUEUE_DEPTH; i++)
    {
        queue->pop(queue->queue, &value);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    diff_ns = (unsigned long long)(end.tv_sec - beg.tv_sec) * 1000 * 1000 * 1000 + (end.tv_nsec - beg.tv_nsec);
    return diff_ns / QUEUE_DEPTH;
}

static double calc_gain(unsigned long long before, unsigned long long after)
{
    return (1.0 - (double)after / (double)before) * 100.0;
}

void run_analysis(params_t *params)
{
    abstract_queue_t queue_1;
    abstract_queue_t queue_2;
    size_t arr_size = sizeof(array_queue_t);
    size_t list_size;
    queue_1.queue = NULL;
    queue_2.queue = NULL;
    int max_len_list = 0;
    unsigned long long time_list, time_array;


    setbuf(stdout, NULL);
    puts("Результаты сравнения:");
    printf("Среднее время моделирования списками: ");
    time_list = run_tests(&queue_1, &queue_2, params, init_list, &max_len_list);
    printf("%llu мкс\n", time_list / 1000);
    printf("Среднее время моделирования массивами: ");
    time_array = run_tests(&queue_1, &queue_2, params, init_array, NULL);
    printf("%llu мкс\n", time_array / 1000);
    printf("Выигрыш времени от очереди на массиве: %.2lf%%\n", calc_gain(time_list, time_array));
    list_size = sizeof(list_node_t) * max_len_list + sizeof(list_queue_t);
    printf("Размер очереди на массиве: %zu байт\n", arr_size);
    printf("Размер очереди на списке: %zu байт\n", list_size);
    printf("Выигрыш памяти от очереди на списке: %.2lf%%\n\n", calc_gain(arr_size, list_size));

    printf("Среднее время добавления элемента в очередь-список: ");
    time_list = run_push(&queue_1, init_list);
    printf("%llu нс\n", time_list);
    printf("Среднее время добавления элемента в очередь-массив: ");
    time_array = run_push(&queue_2, init_array);
    printf("%llu нс\n", time_array);
    printf("Выигрыш времени при добавлении в очередь на массиве: %.2lf%%\n", calc_gain(time_list, time_array));
    printf("Среднее время удаления элемента из очереди-списка: ");
    time_list = run_pop(&queue_1);
    printf("%llu нс\n", time_list);
    printf("Среднее время удаления элемента из очереди-массива: ");
    time_array = run_pop(&queue_2);
    printf("%llu нс\n", time_array);
    printf("Выигрыш времени при удалении из очереди на массиве: %.2lf%%\n\n", calc_gain(time_list, time_array));
    
    queue_1.free(queue_1.queue);
    queue_2.free(queue_2.queue);
}
