#ifndef STRUCT_H__

#define STRUCT_H__

#include <stdint.h>

// Ограничения по варианту
#define MANTISSA_LEN 40
#define INT_LEN 30
#define MAX_EXPONENT 99999

// Структура для вещественного числа
typedef struct
{
    int8_t mnt_sign;
    uint8_t mantissa[MANTISSA_LEN];
    int32_t exponent;
} real_t;

// Структура для целого числа
typedef struct
{
    int8_t sign;
    uint8_t digits[INT_LEN];
} int_t;

#endif // #ifndef STRUCT_H__
