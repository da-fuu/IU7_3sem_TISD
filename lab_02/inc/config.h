#ifndef CONFIG_H__
#define CONFIG_H__

typedef enum
{
    EXIT = 1,
    READ_TABLE,
    SAVE_TABLE,
    PRINT_TABLE,
    PRINT_KEY_TABLE,
    SORT_TABLE,
    SORT_KEY_TABLE,
    SORT_TABLE_WITH_KEY_TABLE,
    ADD_RECORD,
    DELETE_RECORD,
    PRINT_FRIENDS_WITH_NEAR_BDAY,
    RUN_ANALYSIS
} menu_item_t;

#define MAX_DAY 31
#define MAX_MONTH 12
#define MAX_YEAR 10000

#define FILENAME_LEN 30

#endif // #ifndef CONFIG_H__
