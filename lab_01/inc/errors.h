#ifndef ERRORS_H__

#define ERRORS_H__

// Заголовочный файл файла для обработки ошибок


#define OK 0
#define ERR_EMPTY_INPUT 1
#define ERR_INPUT_TOO_LONG 2
#define ERR_INPUT_INVALID 3
#define WARN_ZERO 4
#define WARN_INF 5

// Функция обработки кода ошибки и печати соответствующего сообщения
// Принимает и возвращает код ошибки (позволяет при необходимости 
// изменить код ошибки)
int process_error(int err_code);


#endif	// #ifndef ERRORS_H__
