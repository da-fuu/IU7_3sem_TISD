#include <stdlib.h>
#include <string.h>

#include "list_stack.h"
#include "array_stack.h"
#include "process.h"

static bool is_add(char chr)
{
    return chr == '+' || chr == '-';
}

static double calc_ans(double operand_1, char operator, double operand_2)
{
    switch (operator)
    {
        case '+':
            return operand_1 + operand_2;
        case '-':
            return operand_1 - operand_2;
        case '*':
            return operand_1 * operand_2;
        case '/':
            return operand_1 / operand_2;
        default:
            return 1.0 / 0.0;
    }
}

static void move_stacks(stack_t *in_stack, stack_t *out_stack)
{
    token_t token;
    while (in_stack->pop(in_stack->stack, &token))
        out_stack->push(out_stack->stack, token);
    out_stack->pop(out_stack->stack, &token);
    in_stack->push(in_stack->stack, token);
}

void solve_problem(stack_t *in_stack, stack_t *out_stack)
{
    token_t operand_1, operand_2, operator;

    move_stacks(in_stack, out_stack);

    while (out_stack->pop(out_stack->stack, &operator))
    {
        out_stack->pop(out_stack->stack, &operand_2);
        if (is_add(operator.value.operator))
        {
            in_stack->push(in_stack->stack, operator);
            in_stack->push(in_stack->stack, operand_2);
            continue;
        }
        in_stack->pop(in_stack->stack, &operand_1);
        operand_1.value.operand = calc_ans(operand_1.value.operand, operator.value.operator, operand_2.value.operand);
        in_stack->push(in_stack->stack, operand_1);
    }

    move_stacks(in_stack, out_stack);

    while (out_stack->pop(out_stack->stack, &operator))
    {
        out_stack->pop(out_stack->stack, &operand_2);
        in_stack->pop(in_stack->stack, &operand_1);
        operand_1.value.operand = calc_ans(operand_1.value.operand, operator.value.operator, operand_2.value.operand);
        in_stack->push(in_stack->stack, operand_1);
    }
}

static bool parse_problem(const char *str, stack_t *stack)
{
    char *end_num;
    token_t token;
    bool last_was_operator = true;

    while (*str != '\0')
    {
        if (last_was_operator)
        {
            token.value.operand = strtod(str, &end_num);
            if (str == end_num)
                return false;

            token.is_operator = false;

            str = end_num;
            last_was_operator = false;
        }
        else
        {
            if (!is_operator(*str))
                return false;

            token.is_operator = true;
            token.value.operator = *str;

            str++;
            last_was_operator = true;
        }
        if (!stack->push(stack->stack, token))
            return false;
    }
    return true;
}

bool is_operator(char chr)
{
    return chr == '+' || chr == '-' || chr == '*' || chr == '/';
}

bool solve_problem_from_str(const char *str, double *out)
{
    stack_t stack_1;
    stack_t stack_2;
    token_t answer;

    srand(strlen(str));
    stack_1.stack = NULL;
    if (rand() % 2 == 0)
        init_array(&stack_1);
    else
        init_list(&stack_1);

    if (!parse_problem(str, &stack_1))
    {
        stack_1.free(stack_1.stack);
        return false;
    }

    stack_2.stack = NULL;
    if (rand() % 2 == 0)
        init_array(&stack_2);
    else
        init_list(&stack_2);

    solve_problem(&stack_1, &stack_2);

    stack_1.pop(stack_1.stack, &answer);
    *out = answer.value.operand;

    stack_1.free(stack_1.stack);
    stack_2.free(stack_2.stack);
    return true;
}