#include <stdio.h>
#include "errors.h"
#include "struct.h"
#include "input.h"
#include "print.h"
#include "calc.h"


// Основная функция программы
int main(void)
{
    int ret_code;
    real_t real_num = { 0 };
    int_t integer = { 0 };

    puts("Программа для умножения вещественного числа на целое.");

    greet_real(MANTISSA_LEN);
    ret_code = read_real(&real_num);
    if (process_error(ret_code) != OK)
        return ret_code;

    greet_int(INT_LEN);
    ret_code = read_int(&integer);
    if (process_error(ret_code) != OK)
        return ret_code;

    ret_code = multiply_real_by_int(&real_num, &integer);
    if (process_error(ret_code) != OK)
        return ret_code;

    printf("Результат умножения (вещественное число в нормализованной форме до %d знаков в мантиссе):\n", OUT_MNT_LEN);
    print_real(&real_num);

    return OK;
}
