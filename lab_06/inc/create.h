#ifndef CREATE_H__
#define CREATE_H__

#include "tree.h"

int tree_change_sort(tree_t *tree, bool verbose);

tree_node_t *create_node(file_t *file);

#endif // #ifndef CREATE_H__
