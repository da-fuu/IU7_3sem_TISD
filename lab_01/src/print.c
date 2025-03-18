#include <stdio.h>
#include "print.h"


// Функция печати линейки заданной длины
static void print_liner(int length)
{
    printf("+");
    for (int i = 1; i <= length; i++)
        if (i % 10 == 0)
            printf("%d", i / 10);
        else
            printf("-");
    printf("\n");
}

// Функция печати приветствия для целого числа
void greet_real(int liner_len)
{
    puts("Введите вещественное число, описываемое выражением [+-]?[0-9]*\\.?[0-9]*(E[-+]?[0-9]+)? ,");
    printf("где сумма количества значащих цифр до и после точки в мантиссе <= %d, а модуль порядка <= %d\n", MANTISSA_LEN, MAX_EXPONENT);
    print_liner(liner_len);
}

// Функция печати приветствия для вещественного числа
void greet_int(int liner_len)
{
    puts("Введите целое число, описываемое выражением [+-]?[0-9]+ ,");
    printf("где сумма значащих цифр <= %d\n", INT_LEN);
    print_liner(liner_len);
}

// Функция печати вещественного числа
void print_real(real_t *real_num)
{
    size_t len = MANTISSA_LEN;

    while (real_num->mantissa[len - 1] == 0)
        len--;

    if (real_num->mnt_sign == -1)
        printf("-");
    else
        printf("+");

    printf("0.");
    for (size_t i = 0; i < len; i++)
        printf("%u", real_num->mantissa[i]);

    printf("E");

    printf("%+d\n", real_num->exponent);
}

