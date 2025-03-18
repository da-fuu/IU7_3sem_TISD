#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "errors.h"
#include "struct.h"
#include "input.h"


// Функция удаления начала строки
static void delete_start(char *str, size_t start_shift)
{
    size_t i = 0;
    do
    {
        str[i] = str[i + start_shift];
    }
    while (str[i++ + start_shift]);
}

// Функция проверки строки на то, что она состоит из цифр
static bool check_for_digits(char *str)
{
    while (*str)
        if (!isdigit(*str++))
            return false;
    return true;
}

// Функция чтения целого числа из потока ввода
int read_int(int_t *int_num)
{
    char buffer[BUFFER_LEN];
    size_t len;
    size_t start_shift = 0;

    if (fgets(buffer, BUFFER_LEN, stdin) == NULL)
        return ERR_EMPTY_INPUT;

    len = strlen(buffer);
    if (len <= 1)
        return ERR_EMPTY_INPUT;
    if (buffer[len - 1] != '\n')
        return ERR_INPUT_TOO_LONG;
    buffer[--len] = '\0';

    if (buffer[0] == '-')
    {
        int_num->sign = -1;
        start_shift++;
    }
    else if (buffer[0] == '+')
    {
        int_num->sign = 1;
        start_shift++;
    }
    else
        int_num->sign = 1;

    while (len - start_shift > 1 && buffer[start_shift] == '0')
        start_shift++;

    if (start_shift > 0)
        delete_start(buffer, start_shift);
    if (!check_for_digits(buffer))
        return ERR_INPUT_INVALID;

    len = strlen(buffer);
    if (len > INT_LEN)
        return ERR_INPUT_TOO_LONG;

    for (size_t i = 0; i < len; i++)
        int_num->digits[INT_LEN - i - 1] = buffer[len - i - 1] - '0';

    return OK;
}

// Функция проверки вещественного числа на корректность
static bool check_real(char *str)
{
    bool has_E = false;
    bool has_dot = false;
    if (*str == '-' || *str == '+')
        str++;
    while (*str)
    {
        if (!isdigit(*str))
        {
            if (*str == '.')
                if (has_dot || has_E)
                    return false;
                else
                    has_dot = true;
            else if (*str == 'E')
                if (has_E)
                    return false;
                else
                {
                    has_E = true;
                    if (*(str + 1) == '+' || *(str + 1) == '-')
                        str++;
                }
            else
                return false;
        }
        str++;
    }
    return true;
}

// Функция удаления элемента из строки по индексу
static void remove_char(char *str, size_t index)
{
    size_t len = strlen(str);
    for (size_t i = index; i < len; i++)
        str[i] = str[i + 1];
}

// Функция чтения вещественного числа из потока ввода
int read_real(real_t *real_num)
{
    char buffer[BUFFER_LEN];
    int len;
    size_t start_shift = 0;
    char *exp_ptr;
    int dot_ind;
    int tmp;

    if (fgets(buffer, BUFFER_LEN, stdin) == NULL)
        return ERR_EMPTY_INPUT;

    len = strlen(buffer);
    if (len <= 1)
        return ERR_EMPTY_INPUT;
    if (buffer[len - 1] != '\n')
        return ERR_INPUT_TOO_LONG;
    buffer[--len] = '\0';

    if (!check_real(buffer))
        return ERR_INPUT_INVALID;

    if (buffer[0] == '-')
    {
        real_num->mnt_sign = -1;
        start_shift++;
    }
    else if (buffer[0] == '+')
    {
        real_num->mnt_sign = 1;
        start_shift++;
    }
    else
        real_num->mnt_sign = 1;

    while (buffer[start_shift] == '0')
        start_shift++;

    if (start_shift > 0)
        delete_start(buffer, start_shift);

    exp_ptr = strchr(buffer, 'E');
    if (exp_ptr != NULL)
    {
        *exp_ptr = '\0';
        exp_ptr++;
        if (*exp_ptr == '\0')
            return ERR_INPUT_INVALID;
        tmp = atoi(exp_ptr);
        if (abs(tmp) > MAX_EXPONENT)
            return ERR_INPUT_INVALID;
        real_num->exponent = tmp;
    }
    else
        real_num->exponent = 0;

    len = strlen(buffer);
    dot_ind = strcspn(buffer, ".");
    if (dot_ind < len)
    {
        while (buffer[len - 1] == '0')
            buffer[--len] = '\0';

        remove_char(buffer, dot_ind);
        len--;
        if (len > MANTISSA_LEN)
            return ERR_INPUT_TOO_LONG;
        while (*buffer == '0')
        {
            remove_char(buffer, 0);
            dot_ind--;
            len--;
        }
    }
    else if (len > MANTISSA_LEN)
        return ERR_INPUT_TOO_LONG;
    real_num->exponent += dot_ind;

    for (int i = 0; i < len; i++)
        real_num->mantissa[i] = buffer[i] - '0';

    if (real_num->mantissa[0] == 0)
        real_num->exponent = 0;

    return OK;
}
