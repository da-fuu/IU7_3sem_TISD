#include <stdio.h>

#include "print.h"

void print_token(token_t *token)
{
    if (token->is_operator)
        printf("%c\n", token->value.operator);
    else
        printf("%lf\n", token->value.operand);
}
