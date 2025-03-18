#include "errors.h"
#include "calc.h"


// Функция для умножения вещественного числа на целое(изменяет структуру по указателю)
int multiply_real_by_int(real_t *real_num, int_t *int_num)
{
    uint8_t buffer[MANTISSA_LEN + INT_LEN + 1] = { 0 };
    int start = -1;
    int end, len;
    int ind;
    int new_exp;

    for (int int_ind = INT_LEN - 1; int_ind >= 0; int_ind--)
    {
        for (int mnt_ind = MANTISSA_LEN - 1; mnt_ind >= 0; mnt_ind--)
            buffer[int_ind + mnt_ind + 1] += real_num->mantissa[mnt_ind] * int_num->digits[int_ind];
        for (int buf_ind = MANTISSA_LEN + INT_LEN - 1; buf_ind > 0; buf_ind--)
            if (buffer[buf_ind] > 9)
            {
                buffer[buf_ind - 1] += buffer[buf_ind] / 10;
                buffer[buf_ind] %= 10;
            }
    }
    for (int i = 0; i < MANTISSA_LEN + INT_LEN + 1; i++)
        if (buffer[i] != 0)
        {
            end = i;
            if (start == -1)
                start = i;
        }
    if (start == -1)
    {
        real_num->mnt_sign = 1;
        real_num->exponent = 0;
        for (int i = 0; i < MANTISSA_LEN; i++)
            real_num->mantissa[i] = 0;
        return OK;
    }

    len = end - start + 1;
    if (len > OUT_MNT_LEN)
    {
        end = start + OUT_MNT_LEN - 1;
        if (buffer[end + 1] >= 5)
            buffer[end] += 1;
        ind = end;
        while (buffer[ind] == 10)
        {
            buffer[ind] = 0;
            buffer[ind - 1] += 1;
            ind--;
        }
        for (int i = 0; i < MANTISSA_LEN + INT_LEN + 1; i++)
            if (buffer[i] != 0)
            {
                start = i;
                break;
            }
    }

    new_exp = INT_LEN - start;

    real_num->mnt_sign *= int_num->sign;
    real_num->exponent += new_exp;

    for (int i = 0; i < MANTISSA_LEN; i++)
        if (i < OUT_MNT_LEN)
            real_num->mantissa[i] = buffer[i + start];
        else
            real_num->mantissa[i] = 0;

    if (real_num->exponent > MAX_EXPONENT)
        return WARN_INF;
    if (real_num->exponent < -MAX_EXPONENT)
        return WARN_ZERO;

    return OK;
}