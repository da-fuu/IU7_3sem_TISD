#ifndef STRUCT_H__
#define STRUCT_H__

#include <stddef.h>


#define MAX_TABLE_LEN 1000

#define LASTNAME_LEN 20
#define NAME_LEN 20
#define NUMBER_LEN 11
#define ADDRESS_LEN 25
#define JOB_LEN 25
#define ORGANISATION_LEN 25


typedef struct
{
    int day;
    int month;
    int year;
} date_t;

typedef struct
{
    char value[LASTNAME_LEN + 1];
    int index;
} key_elem_t;

typedef struct
{
    char lastname[LASTNAME_LEN + 1];
    char name[NAME_LEN + 1];
    char number[NUMBER_LEN + 1];
    char address[ADDRESS_LEN + 1];
    enum
    {
        FRIEND = 0,
        COLLEAGUE
    } status;
    union
    {
        date_t birthday;
        struct
        {
            char job[JOB_LEN + 1];
            char organisation[ORGANISATION_LEN + 1];
        } position;
    } info;
} subscriber_t;

typedef struct
{
    key_elem_t keys[MAX_TABLE_LEN];
    int len;
} key_table_t;

typedef struct
{
    subscriber_t subscribers[MAX_TABLE_LEN];
    int len;
} table_t;


#endif // #ifndef STRUCT_H__
