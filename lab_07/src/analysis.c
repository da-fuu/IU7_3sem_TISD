#include <time.h>
#include <stdio.h>
#include <string.h>

#include "struct.h"
#include "input.h"
#include "avl_tree.h"
#include "bst_tree.h"
#include "open_hashtable.h"
#include "closed_hashtable.h"
#include "analysis.h"

#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))


static unsigned long long run_insert(assoc_array_t *assoc_array, void (*init)(assoc_array_t *), char *words[], int len, int *comps)
{
    struct timespec beg, end;
    unsigned long long diff_ns;
    reserved_word_info_t word_info;
    word_info.help = NULL;

    init(assoc_array);

    FILE *file = fopen("big.txt", "r");
    read_words(*assoc_array, file);
    fclose(file);
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &beg);
    for (int i = 0; i < len; i++)
    {
        strcpy(word_info.word, words[i]);
        assoc_array->insert(assoc_array->data, &word_info, comps);
    }    
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    diff_ns = (unsigned long long)(end.tv_sec - beg.tv_sec) * 1000 * 1000 * 1000 + (end.tv_nsec - beg.tv_nsec);
    return diff_ns;
}

static unsigned long long run_find(assoc_array_t *assoc_array, void (*init)(assoc_array_t *), char *words[], int len, int *comps)
{
    struct timespec beg, end;
    unsigned long long diff_ns;
    int tmp = 0;
    reserved_word_info_t word_info;
    word_info.help = NULL;

    init(assoc_array);

    FILE *file = fopen("big.txt", "r");
    read_words(*assoc_array, file);
    fclose(file);

    for (int i = 0; i < len; i++)
    {
        strcpy(word_info.word, words[i]);
        assoc_array->insert(assoc_array->data, &word_info, &tmp);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &beg);
    for (int i = 0; i < len; i++)
    {
        assoc_array->find(assoc_array->data, words[i], comps);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    diff_ns = (unsigned long long)(end.tv_sec - beg.tv_sec) * 1000 * 1000 * 1000 + (end.tv_nsec - beg.tv_nsec);
    return diff_ns;
}

static unsigned long long int run_tests_insert(assoc_array_t *assoc_array, void (*init)(assoc_array_t *), char *words[], int len, int *comps)
{
    unsigned long long res = 0;
    int tmp;

    for (int i = 0; i < START_GARBAGE_NUM; i++)
        run_insert(assoc_array, init, words, len, &tmp);

    for (int i = 0; i < TESTS; i++)
        res += run_insert(assoc_array, init, words, len, comps);

    res /= TESTS;
    return res;
}

static unsigned long long int run_tests_find(assoc_array_t *assoc_array, void (*init)(assoc_array_t *), char *words[], int len, int *comps)
{
    unsigned long long res = 0;
    int tmp;

    for (int i = 0; i < START_GARBAGE_NUM; i++)
        run_find(assoc_array, init, words, len, &tmp);

    for (int i = 0; i < TESTS; i++)
        res += run_find(assoc_array, init, words, len, comps);

    res /= TESTS;
    return res;
}


int run_analysis(void)
{
    char *words[] = {
            "int", "char", "if", "while", "for", "return", "continue", "break", "short", "long",
            "unsigned", "static", "auto", "extern", "void", "else", "switch", "case", "float ", "double"
    };
    assoc_array_t assoc_array;
    assoc_array.data = NULL;
    
    unsigned long long results[4][2];
    double comps[4][2];
    int tmp;

    tmp = 0;
    results[0][0] = run_tests_insert(&assoc_array, init_bst_tree, words, ARR_LEN(words), &tmp);
    comps[0][0] = (double)(tmp / TESTS) / ARR_LEN(words); 
    tmp = 0;
    results[0][1] = run_tests_find(&assoc_array, init_bst_tree, words, ARR_LEN(words), &tmp);
    comps[0][1] = (double)(tmp / TESTS) / ARR_LEN(words); 
    tmp = 0;
    results[1][0] = run_tests_insert(&assoc_array, init_avl_tree, words, ARR_LEN(words), &tmp);
    comps[1][0] = (double)(tmp / TESTS) / ARR_LEN(words); 
    tmp = 0;
    results[1][1] = run_tests_find(&assoc_array, init_avl_tree, words, ARR_LEN(words), &tmp);
    comps[1][1] = (double)(tmp / TESTS) / ARR_LEN(words);
    tmp = 0;
    results[2][0] = run_tests_insert(&assoc_array, init_open_hashtable, words, ARR_LEN(words), &tmp);
    comps[2][0] = (double)(tmp / TESTS) / ARR_LEN(words); 
    tmp = 0;
    results[2][1] = run_tests_find(&assoc_array, init_open_hashtable, words, ARR_LEN(words), &tmp);
    comps[2][1] = (double)(tmp / TESTS) / ARR_LEN(words);
    tmp = 0;
    results[3][0] = run_tests_insert(&assoc_array, init_closed_hashtable, words, ARR_LEN(words), &tmp);
    comps[3][0] = (double)(tmp / TESTS) / ARR_LEN(words); 
    tmp = 0;
    results[3][1] = run_tests_find(&assoc_array, init_closed_hashtable, words, ARR_LEN(words), &tmp);
    comps[3][1] = (double)(tmp / TESTS) / ARR_LEN(words); 
    
    puts("Результаты сравнения (в наносекундах):");
    printf("Двоичное дерево поиска\n");
    printf("|  Операция  | Время | Сравнения |\n");
    printf("| Добавление | %5llu | %9.2lf |\n", results[0][0], comps[0][0]);
    printf("|   Поиск    | %5llu | %9.2lf |\n", results[0][1], comps[0][1]);
    printf("Занимаемый объем памяти: %zu байт\n", sizeof(bst_tree_node_t) * 1000);
    printf("AVL-дерево\n");
    printf("|  Операция  | Время | Сравнения |\n");
    printf("| Добавление | %5llu | %9.2lf |\n", results[1][0], comps[1][0]);
    printf("|   Поиск    | %5llu | %9.2lf |\n", results[1][1], comps[1][1]);
    printf("Занимаемый объем памяти: %zu байт\n", sizeof(avl_tree_node_t) * 1000);
    printf("Хеш-таблица с открытым хешированием\n");
    printf("|  Операция  | Время | Сравнения |\n");
    printf("| Добавление | %5llu | %9.2lf |\n", results[2][0], comps[2][0]);
    printf("|   Поиск    | %5llu | %9.2lf |\n", results[2][1], comps[2][1]);
    printf("Занимаемый объем памяти: %zu байт\n", (sizeof(open_ht_node_t*) + sizeof(open_ht_node_t)) * 1000);
    printf("Хеш-таблица с закрытым хешированием\n");
    printf("|  Операция  | Время | Сравнения |\n");
    printf("| Добавление | %5llu | %9.2lf |\n", results[3][0], comps[3][0]);
    printf("|   Поиск    | %5llu | %9.2lf |\n", results[3][1], comps[3][1]);
    printf("Занимаемый объем памяти: %zu байт\n", (sizeof(closed_ht_elem_t)) * 1000);

    assoc_array.free(&assoc_array.data);
    return OK;
}
