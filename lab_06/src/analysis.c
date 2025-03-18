#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "input.h"
#include "create.h"
#include "analysis.h"
#include "errors.h"

#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))


static unsigned long long run_one_test(tree_t *tree, int type, file_t *sample)
{
    struct timespec beg, end;
    unsigned long long diff_ns;

    tree->root = NULL;
    tree->sorted_by_date = false;
    if (read_file(tree, "big.txt") == ERR_MEM || tree->root == NULL)
        exit(ERR_MEM);

    switch (type)
    {
        case 0:
            clock_gettime(CLOCK_MONOTONIC_RAW, &beg);
            tree->root = wrong_bst_del_old_files(tree->root, sample);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            break;
        case 1:
            tree_change_sort(tree, false);
            clock_gettime(CLOCK_MONOTONIC_RAW, &beg);
            tree->root = correct_bst_del_old_files(tree->root, sample);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            break;
        case 2:
            clock_gettime(CLOCK_MONOTONIC_RAW, &beg);
            tree_change_sort(tree, false);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            break;
    }

    bin_tree_free(&tree->root, true);
    
    diff_ns = (unsigned long long)(end.tv_sec - beg.tv_sec) * 1000 * 1000 * 1000 + (end.tv_nsec - beg.tv_nsec);
    return diff_ns;
}

static unsigned long long run_tests(tree_t *tree, int type, file_t *sample)
{
    unsigned long long res = 0;

    for (int i = 0; i < START_GARBAGE_NUM; i++)
        run_one_test(tree, type, sample);

    for (int i = 0; i < TESTS; i++)
        res += run_one_test(tree, type, sample);

    res /= TESTS;
    return res;
}

static double calc_gain(unsigned long long before, unsigned long long after)
{
    return (1.0 - (double)after / (double)before) * 100.0;
}

int run_analysis(void)
{
    file_t cases[] = { 
        { .date = { 1, 1, 1969 }, .name = "Остаются все элементы       " },
        { .date = { 1, 1, 2000 }, .name = "Удаляется половина элементов" }, 
        { .date = { 1, 1, 2041 }, .name = "Удаляются все элементы      " } 
    };
    
    unsigned long long results[4];
    double gain;
    tree_t tree;

    puts("Результаты сравнения (в наносекундах):");
    printf("|       Тестовые случаи        | Время удаления в изначальном дереве | Время перестроения | Время удаления в перестроенном | Сумма в перестроенном | Выигрыш времени от перестроения |\n");

    for (size_t i = 0; i < ARR_LEN(cases); i++)
    {
        results[0] = run_tests(&tree, 0, &cases[i]);
        results[1] = run_tests(&tree, 2, &cases[i]);
        results[2] = run_tests(&tree, 1, &cases[i]);
        results[3] = results[1] + results[2];
        
        gain = calc_gain(results[0], results[3]);
        printf("| %s | %35llu | %18llu | %30llu | %21llu | %30.2lf%% |\n", cases[i].name, results[0], results[1], results[2], results[3], gain);
    }
    return OK;
}
