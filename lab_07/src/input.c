#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "menu.h"
#include "input.h"



bool read_string(FILE *file, char **str)
{
    size_t len = 0;
    *str = NULL;
    if (getline(str, &len, file) == -1)
    {
        free(*str);
        return false;
    }

    len = strcspn(*str, "\n");
    if (len < 1)
    {
        free(*str);
        return false;
    }

    (*str)[len] = '\0';
    *str = realloc(*str, len + 1);
    return true;
}


int parse_int(const char *str, int max)
{
    long long tmp_int;
    char *endptr;

    if (!isdigit(str[0]))
        return -1;
    tmp_int = strtoll(str, &endptr, 10);
    if (*endptr != '\0')
        return -1;
    if (tmp_int >= max || tmp_int < 0)
        return -1;
    return (int)tmp_int;
}

bool read_int(FILE *file, int *num, int max)
{
    char *buf;

    if (!read_string(file, &buf))
        return false;

    *num = parse_int(buf, max);
    free(buf);
    if (*num <= 0)
        return false;
    return true;
}

bool read_words(assoc_array_t assoc_array, FILE *file)
{
    char *str;
    int tmp, size;
    reserved_word_info_t word_info;
    
    if (!read_int(file, &size, 100000))
        return false;
    
    assoc_array.clean_and_set_size(assoc_array.data, size);

    for (int i = 0; i < size; i++)
    {
        if (!read_string(file, &str) || strlen(str) >= MAX_WORD_LEN)
            return false;

        strcpy(word_info.word, str);
        free(str);
        if (!read_string(file, &str))
            return false;

        word_info.help = str;

        if (!assoc_array.insert(assoc_array.data, &word_info, &tmp))
            return false;
    }
    return true;
}

int read_file_from_user(assoc_array_t assoc_array)
{
    char *filename;
    FILE *file;

    puts("Введите имя файла с зарезервированными словами языка C++:");
    if (!read_string(stdin, &filename))
    {
        puts("Ошибка ввода имени файла!");
        return ERR_CRITICAL;
    }

    if (!(file = fopen(filename, "r")))
    {
        puts("Ошибка чтения файла!");
        free(filename);
        return OK;
    }
    free(filename);

    if (!read_words(assoc_array, file))
    {
        fclose(file);
        return OK;
    }
    fclose(file);
    puts("Слова успешно считаны");
    return OK;
}

int insert_word_from_user(assoc_array_t assoc_array)
{
    char *str;
    int comps = 0;
    reserved_word_info_t word_info;

    puts("Введите зарезервированное слово");
    if (!read_string(stdin, &str) || strlen(str) >= MAX_WORD_LEN)
    {
        puts("Ошибка ввода зарезервированного слова!");
        return ERR_CRITICAL;
    }
    
    strcpy(word_info.word, str);
    free(str);
    puts("Введите подсказку");
    if (!read_string(stdin, &str))
    {
        puts("Ошибка ввода зарезервированного слова!");
        return ERR_CRITICAL;
    }
    word_info.help = str;
    
    if (!assoc_array.insert(assoc_array.data, &word_info, &comps))
    {
        free(word_info.help);
        puts("Слово уже есть в СД!");
        return OK;
    }
    printf("Слово успешно добавлено за %d сравнений\n", comps);
    assoc_array.restruct(assoc_array.data, comps);
    return OK;
}

int find_word_from_user(assoc_array_t assoc_array)
{
    char *reserved_word;
    char *help;
    int comps = 0;
    
    puts("Введите зарезервированное слово");
    if (!read_string(stdin, &reserved_word))
    {
        puts("Ошибка ввода зарезервированного слова!");
        return ERR_CRITICAL;
    }
    if (strlen(reserved_word) >= MAX_WORD_LEN)
    {
        free(reserved_word);
        puts("Ошибка ввода зарезервированного слова!");
        return ERR_CRITICAL;
    }
    
    help = assoc_array.find(assoc_array.data, reserved_word, &comps);
    if (help == NULL)
    {
        free(reserved_word);
        puts("Зарезервированное слово не найдено!");
        return OK;
    }
    free(reserved_word);
    printf("Подсказка:\n%s\nНайдена за %d сравнений\n", help, comps);
    assoc_array.restruct(assoc_array.data, comps);
    return OK;
}

int remove_word_from_user(assoc_array_t assoc_array)
{
    char *reserved_word;
    int comps = 0;

    puts("Введите зарезервированное слово");
    if (!read_string(stdin, &reserved_word))
    {
        puts("Ошибка ввода зарезервированного слова!");
        return ERR_CRITICAL;
    }
    if (strlen(reserved_word) >= MAX_WORD_LEN)
    {
        free(reserved_word);
        puts("Ошибка ввода зарезервированного слова!");
        return ERR_CRITICAL;
    }

    if (!assoc_array.remove(assoc_array.data, reserved_word, &comps))
    {
        free(reserved_word);
        puts("Зарезервированное слово не найдено!");
        return OK;
    }
    free(reserved_word);
    printf("Слово удалено за %d сравнений\n", comps);
    assoc_array.restruct(assoc_array.data, comps);
    return OK;
}

int export_from_user(assoc_array_t assoc_array)
{
    assoc_array.print(assoc_array.data);
    return OK;
}

int change_comparisons_from_user(assoc_array_t assoc_array)
{
    int comps;
    if (!assoc_array.set_comps(assoc_array.data, -1))
        return OK;

    puts("Введите максимальное число сравнений");
    if (!read_int(stdin, &comps, 10000))
    {
        puts("Ошибка ввода числа сравнений!");
        return ERR_CRITICAL;
    }
    assoc_array.set_comps(assoc_array.data, comps);
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

