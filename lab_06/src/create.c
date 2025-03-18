#include <stdlib.h>

#include "tree.h"
#include "errors.h"
#include "create.h"


tree_node_t *create_node(file_t *file)
{
    tree_node_t *node = malloc(sizeof(tree_node_t));
    if (!node)
        return NULL;
    node->right = NULL;
    node->left = NULL;
    node->file = *file;
    return node;
}

int tree_copy_in_sorted(tree_node_t *tree_in, tree_t *tree_out)
{
    int rc;
    tree_node_t *new = create_node(&tree_in->file);
    if (!new)
        return ERR_MEM;
    rc = universal_tree_insert(tree_out, new);
    if (rc != OK)
    {
        puts("Невозможно сменить вид дерева!");
        return rc;
    }
    if (tree_in->left)
    {
        rc = tree_copy_in_sorted(tree_in->left, tree_out);
        if (rc != OK)
            return rc;
    }
    if (tree_in->right)
    {
        rc = tree_copy_in_sorted(tree_in->right, tree_out);
        if (rc != OK)
            return rc;
    }
    return OK;
}

int tree_change_sort(tree_t *tree, bool verbose)
{
    tree_t new;
    int rc;
    
    new.sorted_by_date = !tree->sorted_by_date;
    new.root = NULL;
    rc = tree_copy_in_sorted(tree->root, &new);
    if (rc != OK)
    {
        bin_tree_free(&new.root, false);
        if (rc == ERR_ELEM_ALREADY)
        {
            return OK;
        }
        return rc;
    }
    bin_tree_free(&tree->root, false);
    *tree = new;
    if (verbose)
        puts("Дерево успешно перестроено");
    return OK;
}
