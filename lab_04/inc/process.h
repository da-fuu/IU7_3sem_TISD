#ifndef PROCESS_H__
#define PROCESS_H__

#include <stdbool.h>

#include "struct.h"


void solve_problem(stack_t *in_stack, stack_t *out_stack);

bool solve_problem_from_str(const char *str, double *out);

bool is_operator(char chr);


#endif // #ifndef PROCESS_H__
