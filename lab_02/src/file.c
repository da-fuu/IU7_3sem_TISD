#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "config.h"
#include "file.h"


#define ERR_OK 0
#define ERR_END 1
#define ERR_STRUCT 2

#define BUF_LEN 6


static FILE *input_filename(char *mode)
{
    char filename[FILENAME_LEN + 1];
    FILE *f = NULL;

    while (f == NULL)
    {
        if (fgets(filename, FILENAME_LEN + 1, stdin) == NULL)
        {
            puts("Введите непустое имя файла!");
            continue;
        }

        if (filename[strlen(filename) - 1] != '\n')
        {
            flush_stdin();
            printf("Введите имя файла не длиннее %d символов!\n", FILENAME_LEN);
            continue;
        }
        filename[strlen(filename) - 1] = '\0';
        f = fopen(filename, mode);
        if (f == NULL)
            puts("Введите корректное имя файла!");
    }
    return f;
}

void flush_stdin(void)
{
    int tmp;
    puts("Ошибка ввода! (Если вы не видите сообщение о причине ошибке, нажмите Enter)");
    do
    {
        tmp = getchar();
        if (tmp == EOF)
        {
            puts("Вы нажали ctrl+d или передали на ввод некорректный файл, не надо так!");
            exit(1);
        }
    }
    while (tmp != '\n');
}

static void write_subscriber(const subscriber_t *subscriber, FILE *file)
{
    fprintf(file, "%s\n%s\n%s\n%s\n", subscriber->lastname, subscriber->name, subscriber->number, subscriber->address);
    fprintf(file, "%d\n", subscriber->status);
    if (subscriber->status == FRIEND)
        fprintf(file, "%d\n%d\n%d\n", subscriber->info.birthday.day, subscriber->info.birthday.month, subscriber->info.birthday.year);
    else if (subscriber->status == COLLEAGUE)
        fprintf(file, "%s\n%s\n", subscriber->info.position.job, subscriber->info.position.organisation);
}

void save_table(const table_t *table)
{
    FILE *file;

    puts("Введите имя файла, в который будет сохранена таблица:");
    file = input_filename("w");

    for (int i = 0; i < table->len; i++)
        write_subscriber(&table->subscribers[i], file);

    puts("Таблица успешно записана в файл");
    fclose(file);
}

static int read_first_line(char *str, int len, FILE *file)
{
    size_t new_len;
    if (fgets(str, len + 1, file) == NULL)
        return ERR_END;
    new_len = strcspn(str, "\n");
    if (new_len < 1)
        return ERR_STRUCT;
    if (str[new_len] != '\n' && fgetc(file) != '\n')
        return ERR_STRUCT;
    str[new_len] = '\0';
    return ERR_OK;
}

bool read_line(char *str, int len, FILE *file)
{
    int rc = read_first_line(str, len, file);
    if (rc != ERR_OK)
        return false;
    return true;
}

bool read_num(int *num, int max_val, FILE *file)
{
    char buf[BUF_LEN];
    char *endptr;
    long tmp;

    if (fgets(buf, BUF_LEN, file) == NULL)
        return false;
    if (!isdigit(buf[0]))
        return false;

    tmp = strtol(buf, &endptr, 10);
    if (endptr != strrchr(buf, '\n'))
        return false;
    if (tmp > max_val || tmp <= 0)
        return false;

    *num = (int)tmp;
    return true;
}

static int read_subscriber(subscriber_t *subscriber, FILE *file)
{
    char buffer[2];
    int status;
    int rc;

    rc = read_first_line(subscriber->lastname, LASTNAME_LEN, file);
    if (rc != ERR_OK)
        return rc;

    if (!read_line(subscriber->name, NAME_LEN, file))
        return ERR_STRUCT;

    if (!read_line(subscriber->number, NUMBER_LEN, file))
        return ERR_STRUCT;
    if (!isdigit(subscriber->number[0]) && subscriber->number[0] != '+')
        return ERR_STRUCT;

    for (size_t i = strlen(subscriber->number) - 1; i > 0; i--)
        if (!isdigit(subscriber->number[i]))
            return ERR_STRUCT;

    if (!read_line(subscriber->address, ADDRESS_LEN, file))
        return ERR_STRUCT;

    if (!read_line(buffer, 1, file))
        return ERR_STRUCT;
    status = buffer[0] - '0';
    if (status != FRIEND && status != COLLEAGUE)
        return ERR_STRUCT;
    subscriber->status = status;

    if (subscriber->status == FRIEND)
    {
        if (!read_num(&subscriber->info.birthday.day, MAX_DAY, file))
            return ERR_STRUCT;
        if (!read_num(&subscriber->info.birthday.month, MAX_MONTH, file))
            return ERR_STRUCT;
        if (!read_num(&subscriber->info.birthday.year, MAX_YEAR, file))
            return ERR_STRUCT;
    }
    if (subscriber->status == COLLEAGUE)
    {
        if (!read_line(subscriber->info.position.job, JOB_LEN, file))
            return ERR_STRUCT;
        if (!read_line(subscriber->info.position.organisation, ORGANISATION_LEN, file))
            return ERR_STRUCT;
    }
    return ERR_OK;
}

bool read_from_file(table_t *table, FILE *file)
{
    int rc;
    subscriber_t tmp;

    table->len = 0;
    do
    {
        if (table->len > MAX_TABLE_LEN)
        {
            puts("Слишком длинный файл!");
            table->len = 0;
            return false;
        }
        rc = read_subscriber(&tmp, file);
        if (table->len < MAX_TABLE_LEN && rc == ERR_OK)
            table->subscribers[table->len] = tmp;
        table->len++;
    }
    while (rc == ERR_OK);
    table->len--;
    if (rc != ERR_END)
    {
        puts("Некорректная структура файла!");
        table->len = 0;
        return false;
    }
    if (table->len == 0)
    {
        puts("Пустой файл!");
        return false;
    }
    return true;
}

void read_table(table_t *table)
{
    FILE *file;

    puts("Введите имя файла с таблицей:");
    file = input_filename("r");

    if (read_from_file(table, file))
        puts("Таблица успешно считана");

    fclose(file);
}
