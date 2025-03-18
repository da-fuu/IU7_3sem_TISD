#include <time.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "file.h"
#include "print.h"
#include "config.h"
#include "operations.h"


static int find_by_lastname(const table_t *table, char *lastname_to_find)
{
    for (int i = 0; i < table->len; i++)
        if (strcmp(table->subscribers[i].lastname, lastname_to_find) == 0)
            return i;
    return -1;
}

static void delete_by_index(table_t *table, int index)
{
    for (int i = index; i < table->len - 1; i++)
        table->subscribers[i] = table->subscribers[i + 1];
    table->len--;
}


void delete_record_from_table(table_t *table)
{
    char lastname[LASTNAME_LEN + 1];
    int index;

    puts("Введите строку - фамилию абонента, которого вы хотите удалить:");
    while (!read_line(lastname, LASTNAME_LEN, stdin))
    {
        flush_stdin();
        printf("Введите фамилию не длиннее %d символов!\n", LASTNAME_LEN);
    }

    index = find_by_lastname(table, lastname);
    if (index < 0)
    {
        puts("Абонент с введенной фамилией не найден");
        return;
    }
    delete_by_index(table, index);
    printf("Удален абонент с (бывшим) ID %d\n", index);
}

static void read_user_subscriber(subscriber_t *subscriber)
{
    char buffer[2];
    int status;
    bool flag;

    puts("Введите строку - фамилию:");
    while (!read_line(subscriber->lastname, LASTNAME_LEN, stdin))
    {
        flush_stdin();
        printf("Введите фамилию не длиннее %d символов!\n", LASTNAME_LEN);
    }

    puts("Введите строку - имя:");
    while (!read_line(subscriber->name, NAME_LEN, stdin))
    {
        flush_stdin();
        printf("Введите имя не длиннее %d символов!\n", NAME_LEN);
    }

    puts("Введите строку - номер в формате [+][0-9]*:");
    flag = false;
    while (!flag)
    {
        if (!read_line(subscriber->number, NUMBER_LEN, stdin))
        {
            flush_stdin();
            printf("Введите номер не длиннее %d символов!\n", NUMBER_LEN);
            continue;
        }
        if (!isdigit(subscriber->number[0]) && subscriber->number[0] != '+')
        {
            flush_stdin();
            printf("Номер может начинаться только с цифры или плюса!\n");
            continue;
        }

        flag = true;
        for (size_t i = strlen(subscriber->number) - 1; i > 0; i--)
            if (!isdigit(subscriber->number[i]))
            {
                flag = false;
                break;
            }
        if (!flag)
        {
            flush_stdin();
            printf("Номер может содержать только цифры!\n");
        }
    }

    puts("Введите строку - адрес:");
    while (!read_line(subscriber->address, ADDRESS_LEN, stdin))
    {
        flush_stdin();
        printf("Введите адрес не длиннее %d символов!\n", ADDRESS_LEN);
    }

    printf("Введите число - статус абонента(%d - друзья, %d - коллеги):\n", FRIEND, COLLEAGUE);
    flag = false;
    while (!flag)
    {
        if (!read_line(buffer, 1, stdin))
        {
            flush_stdin();
            puts("Введите только одну цифру!");
            continue;
        }
        status = buffer[0] - '0';
        if (status != FRIEND && status != COLLEAGUE)
        {
            flush_stdin();
            printf("Введите только %d или %d!\n", FRIEND, COLLEAGUE);
            continue;
        }
        subscriber->status = status;
        flag = true;
    }

    if (subscriber->status == FRIEND)
    {
        printf("Введите число - год рождения:\n");
        while (!read_num(&subscriber->info.birthday.year, MAX_YEAR, stdin))
        {
            flush_stdin();
            printf("Введите год от 1 до %d!\n", MAX_YEAR);
        }
        printf("Введите число - месяц рождения:\n");
        while (!read_num(&subscriber->info.birthday.month, MAX_MONTH, stdin))
        {
            flush_stdin();
            printf("Введите месяц от 1 до %d!\n", MAX_MONTH);
        }
        printf("Введите число - день рождения:\n");
        while (!read_num(&subscriber->info.birthday.day, MAX_DAY, stdin))
        {
            flush_stdin();
            printf("Введите день от 1 до %d!\n", MAX_DAY);
        }
    }
    if (subscriber->status == COLLEAGUE)
    {
        puts("Введите строку - должность:");
        while (!read_line(subscriber->info.position.job, JOB_LEN, stdin))
        {
            flush_stdin();
            printf("Введите должность длиной до до %d!\n", JOB_LEN);
        }
        puts("Введите строку - организацию:");
        while (!read_line(subscriber->info.position.organisation, ORGANISATION_LEN, stdin))
        {
            flush_stdin();
            printf("Введите организацию длиной до до %d!\n", ORGANISATION_LEN);
        }
    }
}

void add_record_to_table(table_t *table)
{
    int new_id = table->len;

    if (new_id >= MAX_TABLE_LEN)
    {
        puts("Таблица уже максимального размера!");
        return;
    }
    table->len++;
    read_user_subscriber(&table->subscribers[new_id]);
    puts("Абонент успешно добавлен в конец таблицы");
}

static void input_curr_date(date_t *curr)
{
    printf("Введите число - текущий год:\n");
    while (!read_num(&curr->year, MAX_YEAR, stdin))
    {
        flush_stdin();
        printf("Введите год от 1 до %d!\n", MAX_YEAR);
    }
    printf("Введите число - текущий месяц:\n");
    while (!read_num(&curr->month, MAX_MONTH, stdin))
    {
        flush_stdin();
        printf("Введите месяц от 1 до %d!\n", MAX_MONTH);
    }
    printf("Введите число - текущий день:\n");
    while (!read_num(&curr->day, MAX_DAY, stdin))
    {
        flush_stdin();
        printf("Введите день от 1 до %d!\n", MAX_DAY);
    }
}

static time_t get_time_from_date(const date_t *date)
{
    struct tm time_prt;
    time_prt.tm_mday = date->day;
    time_prt.tm_mon = date->month - 1;
    time_prt.tm_year = date->year - 1900;
    time_prt.tm_min = 0;
    time_prt.tm_hour = 0;
    time_prt.tm_sec = 0;
    time_prt.tm_isdst = -1;
    return mktime(&time_prt);
}

static int find_diff_in_days(const date_t *future, const date_t *now)
{
    time_t future_sec = get_time_from_date(future);
    time_t now_sec = get_time_from_date(now);
    double diff = difftime(now_sec, future_sec);
    int diff_days = (int) diff / 60 / 60 / 24;
    return diff_days;
}

static date_t find_next_bday(const date_t *today, const date_t *birthday)
{
    date_t next_bday;
    next_bday.year = today->year;
    next_bday.month = birthday->month;
    next_bday.day = birthday->day;

    if (birthday->month < today->month || (birthday->month == today->month && birthday->day < today->day))
        next_bday.year++;
    return next_bday;
}

void print_friends_with_bday(const table_t *table)
{
    date_t curr_date, next_birthday;
    bool flag_found = false;

    input_curr_date(&curr_date);

    for (int i = 0; i < table->len; i++)
        if (table->subscribers[i].status == FRIEND)
        {
            next_birthday = find_next_bday(&curr_date, &table->subscribers[i].info.birthday);
            if (find_diff_in_days(&curr_date, &next_birthday) <= 7)
            {
                if (!flag_found)
                {
                    print_header();
                    flag_found = true;
                }
                print_subscriber(&table->subscribers[i], i);
            }
        }
    if (!flag_found)
        puts("Ни одного друга не надо поздравлять!");
}
