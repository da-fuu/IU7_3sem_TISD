#include <stdio.h>
#include "errors.h"


// Функция обработки кода ошибки и печати соответствующего сообщения
// Принимает и возвращает код ошибки (позволяет при необходимости 
// изменить код ошибки)
int process_error(int err_code)
{
    switch (err_code)
    {
        case OK:
            break;
        case ERR_EMPTY_INPUT:
            puts("Пустой ввод!");
            break;
        case ERR_INPUT_TOO_LONG:
            puts("Слишком большой ввод!");
            break;
        case ERR_INPUT_INVALID:
            puts("Неверный ввод!");
            break;
        case WARN_ZERO:
            puts("Машинный ноль!");
            break;
        case WARN_INF:
            puts("Машинная бесконечность!");
            break;
        default:
            puts("Неизвестная ошибка");
            break;
    }
    return err_code;
}
