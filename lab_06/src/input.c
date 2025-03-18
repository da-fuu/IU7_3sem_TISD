#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "create.h"
#include "menu.h"
#include "errors.h"
#include "input.h"


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


int find_from_user(tree_t tree)
{
    char *name;
    tree_node_t *found;
    int rc;
    
    puts("Введите имя файла к поиску:");
    if (!read_string(stdin, &name))
    {
        puts("Ошибка чтения имени файла!");
        return ERR_EOF;
    }
    rc = universal_tree_find(tree, name, &found);
    if (rc != OK)
    {
        free(name);
        puts("Файл с заданным именем не найден!");
        return OK;
    }
    free(name);
    puts("Информация о найденном файле:");
    print_file(&found->file);
    return OK;
}


int delete_from_user(tree_t *tree)
{
    char *name;
    int rc;
    
    puts("Введите имя файла к удалению:");
    if (!read_string(stdin, &name))
    {
        puts("Ошибка чтения имени файла!");
        return ERR_EOF;
    }
    rc = universal_tree_delete(tree, name);
    if (rc != OK)
    {
        free(name);
        puts("Файл с заданным именем не найден!");
        return OK;
    }
    free(name);
    puts("Файл удален");
    return OK;
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
    if (tmp_int > max || tmp_int <= 0)
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
    if (*num < 0)
        return false;
    return true;
}


int read_struct(FILE *file, file_t *out, bool verbose)
{
    int temp;
    
    if (verbose)
        puts("Введите имя файла:");
    if (!read_string(file, &out->name))
    {
        puts("Ошибка чтения имени файла!");
        return ERR_EOF;
    }
    if (verbose)
        puts("Введите день последнего обращения файла:");
    if (!read_int(file, &temp, 31))
    {
        free(out->name);
        puts("Ошибка чтения даты обращения файла!");
        return ERR_EOF;
    }
    out->date.day = (short)temp;
    if (verbose)
        puts("Введите месяц последнего обращения файла:");
    if (!read_int(file, &temp, 12))
    {
        free(out->name);
        puts("Ошибка чтения даты обращения файла!");
        return ERR_EOF;
    }
    out->date.month = (short)temp;
    if (verbose)
        puts("Введите год последнего обращения файла:");
    if (!read_int(file, &temp, 2100))
    {
        free(out->name);
        puts("Ошибка чтения даты обращения файла!");
        return ERR_EOF;
    }
    out->date.year = temp;
    if (verbose)
        puts("Введите 2 для скрытого файла, иначе 1:");
    if (!read_int(file, &temp, 2))
    {
        free(out->name);
        puts("Ошибка чтения атрибута файла!");
        return ERR_EOF;
    }
    out->hidden = temp - 1;
    if (verbose)
        puts("Введите 2 для системного файла, иначе 1:");
    if (!read_int(file, &temp, 2))
    {
        free(out->name);
        puts("Ошибка чтения атрибута файла!");
        return ERR_EOF;
    }
    out->system = temp - 1;
    
    return OK;
}

int insert_from_user(tree_t *tree)
{
    file_t new;
    tree_node_t *node;
    if (read_struct(stdin, &new, true) != OK)
        return ERR_EOF;
    node = create_node(&new);
    if (!node)
    {
        puts("Нехватка памяти!");
        return ERR_MEM;
    }
    
    if (universal_tree_insert(tree, node) != OK)
    {
        puts("Элемент уже есть в дереве!");
        return OK;
    }
    puts("Элемент добавлен в дерево");
    return OK;
}


int read_file(tree_t *tree, char *filename)
{
    FILE *file;
    int len;
    file_t new;
    tree_node_t *node;
    
    if (!(file = fopen(filename, "r")))
    {
        puts("Ошибка чтения файла!");
        return OK;
    }

    if (!read_int(file, &len, 10000))
    {
        fclose(file);
        puts("Ошибка чтения файла!");
        return OK;
    }
    for (int i = 0; i < len; i++)
    {
        if (read_struct(file, &new, false) != OK)
        {
            bin_tree_free(&tree->root, true);
            fclose(file);
            puts("Ошибка чтения файла!");
            return OK;
        }
        if (!(node = create_node(&new)))
        {
            fclose(file);
            puts("Нехватка памяти!");
            return ERR_MEM;
        }
        if (universal_tree_insert(tree, node) != OK)
        {
            bin_tree_free(&tree->root, true);
            fclose(file);
            puts("Совпадающие элементы в файле!");
            return OK;
        }
    }
    fclose(file);
    return OK;
}


int read_file_from_user(tree_t *tree)
{
    char *filename;
    
    if (tree->root != NULL)
        bin_tree_free(&tree->root, true);
    
    tree->root = NULL;
    tree->sorted_by_date = false;
    
    puts("Введите имя файла:");
    if (!read_string(stdin, &filename))
    {
        puts("Ошибка ввода имени файла!");
        return ERR_EOF;
    }

    if (read_file(tree, filename) == ERR_MEM)
    {
        free(filename);
        return ERR_MEM;
    }
    free(filename);
    return OK;
}

int del_old_files_from_user(tree_t *tree)
{
    file_t sample;
    int temp;
    
    puts("Введите первую не удаляемую дату обращения:");
    puts("Введите день:");
    if (!read_int(stdin, &temp, 31))
    {
        puts("Ошибка чтения даты обращения!");
        return ERR_EOF;
    }
    sample.date.day = (short)temp;
    puts("Введите месяц:");
    if (!read_int(stdin, &temp, 12))
    {
        puts("Ошибка чтения даты обращения!");
        return ERR_EOF;
    }
    sample.date.month = (short)temp;
    puts("Введите год:");
    if (!read_int(stdin, &temp, 2100))
    {
        puts("Ошибка чтения даты обращения!");
        return ERR_EOF;
    }
    sample.date.year = temp;

    universal_del_old_files(tree, &sample);
    
    return OK;
}
