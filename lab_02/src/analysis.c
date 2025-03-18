#include <time.h>
#include <stdio.h>
#include <string.h>
#include "sort.h"
#include "file.h"
#include "key_table.h"
#include "analysis.h"


#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

typedef enum
{
    BUBBLE_SORT = 0,
    QSORT,
    SORT_ALGS_N
} sort_algs_e;

typedef enum
{
    TABLE_SORT = 0,
    KEY_TABLE_SORT,
    SORT_METHODS_N
} sort_methods_e;


static unsigned long long run_sort(void (*sort_func)(void *), void *table)
{
    struct timespec beg, end;
    unsigned long long diff_ns;

    clock_gettime(CLOCK_MONOTONIC_RAW, &beg);
    sort_func(table);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    diff_ns = (unsigned long long)(end.tv_sec - beg.tv_sec) * 1000 * 1000 * 1000 + (end.tv_nsec - beg.tv_nsec);
    return diff_ns;
}

static unsigned long long run_tests_table(void (*sort_func)(void *), const table_t *table)
{
    table_t tmp_table;
    unsigned long long res = 0;

    for (int i = 0; i < START_GARBAGE_NUM; i++)
    {
        memcpy(&tmp_table, table, sizeof(table_t));
        run_sort(sort_func, &tmp_table);
    }

    for (int i = 0; i < TESTS_NUM; i++)
    {
        memcpy(&tmp_table, table, sizeof(table_t));
        res += run_sort(sort_func, &tmp_table);
    }
    res /= TESTS_NUM;
    return res;
}


static unsigned long long run_tests_key_table(void (*sort_func)(void *), const table_t *table)
{
    key_table_t key_table, tmp_key_table;
    unsigned long long res = 0;

    create_key_table(table, &key_table);

    for (int i = 0; i < START_GARBAGE_NUM; i++)
    {
        memcpy(&tmp_key_table, &key_table, sizeof(key_table_t));
        run_sort(sort_func, &tmp_key_table);
    }

    for (int i = 0; i < TESTS_NUM; i++)
    {
        memcpy(&tmp_key_table, &key_table, sizeof(key_table_t));
        res += run_sort(sort_func, &tmp_key_table);
    }
    res /= TESTS_NUM;
    return res;
}

static double calc_gain(unsigned long long before, unsigned long long after)
{
    return (1.0 - (double)after / (double)before) * 100.0;
}

void run_analysis(void)
{
    table_t table;
    int sizes[] = { 40, 200, 500, 1000 };
    unsigned long long results[SORT_ALGS_N][SORT_METHODS_N][ARR_LEN(sizes)];
    double gains[SORT_ALGS_N * SORT_METHODS_N][ARR_LEN(sizes)];
    FILE *file;
    bool ret_flag;

    file = fopen("big_table.txt", "r");
    if (file == NULL)
    {
        puts("Нет необходимого файла с таблицей");
        return;
    }
    ret_flag = read_from_file(&table, file);
    fclose(file);
    if (!ret_flag)
        return;

    if (table.len < sizes[ARR_LEN(sizes) - 1])
    {
        puts("Слишком маленькая таблица в файле для заданных тестируемых размеров!");
        return;
    }
    for (size_t i = 0; i < ARR_LEN(sizes); i++)
    {
        table.len = sizes[i];
        results[BUBBLE_SORT][TABLE_SORT][i] = run_tests_table(slow_sort_table, &table);
        results[BUBBLE_SORT][KEY_TABLE_SORT][i] = run_tests_key_table(slow_sort_key_table, &table);
        results[QSORT][TABLE_SORT][i] = run_tests_table(fast_sort_table, &table);
        results[QSORT][KEY_TABLE_SORT][i] = run_tests_key_table(fast_sort_key_table, &table);
        gains[0][i] = calc_gain(results[BUBBLE_SORT][TABLE_SORT][i], results[BUBBLE_SORT][KEY_TABLE_SORT][i]);
        gains[1][i] = calc_gain(results[QSORT][TABLE_SORT][i], results[QSORT][KEY_TABLE_SORT][i]);
        gains[2][i] = calc_gain(results[BUBBLE_SORT][TABLE_SORT][i], results[QSORT][TABLE_SORT][i]);
        gains[3][i] = calc_gain(results[BUBBLE_SORT][KEY_TABLE_SORT][i], results[QSORT][KEY_TABLE_SORT][i]);
    }

    puts("Результаты сортировок в наносекундах:");

    printf("%s | %34s | %s | %27s | %s |\n", "Размер", "Таблицы пузырьком", "Таблицы ключей пузырьком", "Таблицы qsort", "Таблицы ключей qsort");
    for (size_t i = 0; i < ARR_LEN(sizes); i++)
    {
        printf("%6d | %18llu | %24llu | %20llu | %20llu |\n", sizes[i], results[BUBBLE_SORT][TABLE_SORT][i], results[BUBBLE_SORT][KEY_TABLE_SORT][i],
        results[QSORT][TABLE_SORT][i], results[QSORT][KEY_TABLE_SORT][i]);
    }

    printf("%s | %s | %s | %s | %s |\n", "Размер", "Выигрыш от сортировки таблицы ключей(пузырек)", "Выигрыш от сортировки таблицы ключей(qsort)", "Выигрыш от qsort(всей таблицы)", "Выигрыш от qsort(таблицы ключей)");
    for (size_t i = 0; i < ARR_LEN(sizes); i++)
    {
        printf("%6d | %+44.2lf%% | %+42.2lf%% | %+29.2lf%% | %+31.2lf%% |\n", sizes[i], gains[0][i], gains[1][i], gains[2][i], gains[3][i]);
    }
    for (int i = 0; i < 170; i++)
        printf("-");
    printf("\n");

    printf("Дополнительные затраты памяти при сортировке таблицы ключей: %.2lf%%\n", (double)sizeof(key_table_t) / (double)sizeof(table_t) * 100.0);
}
