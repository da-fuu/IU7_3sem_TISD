#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

typedef struct
{
    int day;
    int month;
    int year;
} date_t;


bool comp(date_t d1, date_t d2)
{
    return d1.year == d2.year && d1.month == d2.month && d1.day == d2.day;
}

bool in(date_t *dates, int n, date_t find)
{
    for (int i = 0; i < n; ++i)
    {
        if (comp(dates[i], find))
            return true;
    }
    return false;
}

int main()
{
    char name[10];
    date_t date;
    int hid, sys;
    date_t dates[1000];
    printf("%d\n", 1000);
    for (int i = 0; i < 1000; ++i)
    {
        name[0] = rand() % ('z' - 'a' + 1) + 'a';
        name[1] = rand() % ('z' - 'a' + 1) + 'a';
        name[2] = rand() % ('z' - 'a' + 1) + 'a';
        name[3] = rand() % ('z' - 'a' + 1) + 'a';
        name[4] = rand() % ('z' - 'a' + 1) + 'a';
        if (rand() < RAND_MAX / 10)
            name[4] = 0;
        name[5] = rand() % ('z' - 'a') + 'a';
        if (rand() < RAND_MAX / 3)
            name[5] = 0;
        name[6] = 0;
        
        date.day = rand() % 30;
        if (!date.day)
            date.day = 30;
        date.month = rand() % 12 + 1;
        date.year = rand() % 70 + 1970;
        if (in(dates, i, date))
            date.year = 2040;
        
        dates[i + 1] = date;

        if (rand() < RAND_MAX / 3)
            hid = 2;
        else
            hid = 1;
        if (rand() < RAND_MAX / 10)
            sys = 2;
        else
            sys = 1;
        
        printf("%s\n%d\n%d\n%d\n%d\n%d\n", name, date.day, date.month, date.year, hid, sys);
    }
}