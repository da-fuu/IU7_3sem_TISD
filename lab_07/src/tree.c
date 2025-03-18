#include <stdio.h>

#include "tree.h"


bool abs_tree_set_comps_stub(void *data, int size)
{
    (void)data;
    (void)size;

    puts("Данное действие не применимо к дереву!");
    return false;
}

void tree_print_dot_null(FILE *f, reserved_word_t name, int null_count)
{
    fprintf(f, "  null%d [shape=point];\n", null_count);
    fprintf(f, "  %s -> null%d;\n", name, null_count);
}


void tree_set_size_stub(void *data, int size)
{
    (void)data;
    (void)size;
}

void tree_restruct_stub(void *data, int size)
{
    (void)data;
    (void)size;
}
