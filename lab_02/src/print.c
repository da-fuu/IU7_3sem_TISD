#include <stdio.h>
#include "sort.h"
#include "print.h"

#define STATUS_LEN 7


void print_subscriber(const subscriber_t *subscriber, int index)
{
    printf("%3d | ", index);
    printf("%*s | %*s | ", LASTNAME_LEN, subscriber->lastname, NAME_LEN, subscriber->name);
    printf("%*s | %*s | ", NUMBER_LEN, subscriber->number, ADDRESS_LEN, subscriber->address);

    if (subscriber->status == FRIEND)
    {
        printf("%*s | ", STATUS_LEN + 4, "Друг");
        printf("%*d.%02d | %*d\n", JOB_LEN - 3, subscriber->info.birthday.day, subscriber->info.birthday.month, ORGANISATION_LEN, subscriber->info.birthday.year);
    }
    else if (subscriber->status == COLLEAGUE)
    {
        printf("%*s | ", STATUS_LEN + 7, "Коллега");
        printf("%*s | %*s\n", JOB_LEN, subscriber->info.position.job, ORGANISATION_LEN, subscriber->info.position.organisation);
    }
}

void print_header(void)
{
    printf("%3s | ", "ID");
    printf("%*s | %*s | ", LASTNAME_LEN + 7, "Фамилия", NAME_LEN + 3, "Имя");
    printf("%*s | %*s | ", NUMBER_LEN + 5, "Номер", ADDRESS_LEN + 5, "Адрес");
    printf("%*s | %*s | %*s\n", STATUS_LEN + 6, "Статус", JOB_LEN + 21, "Дата рождения/Должность", ORGANISATION_LEN + 22, "Год рождения/Организация");
}

void print_table(const table_t *table)
{
    print_header();
    for (int i = 0; i < table->len; i++)
        print_subscriber(&table->subscribers[i], i);
}

static void print_key(const key_elem_t *key_elem)
{
    printf("%*s | %d\n", LASTNAME_LEN, key_elem->value, key_elem->index);
}

void print_key_table(const key_table_t *key_table)
{
    printf("%*s | %s\n", LASTNAME_LEN + 7, "Фамилия", "Индекс в исходной матрице");
    for (int i = 0; i < key_table->len; i++)
        print_key(&key_table->keys[i]);
}

void print_sorted_table(table_t *table)
{
    fast_sort_table(table);
    print_table(table);
}
