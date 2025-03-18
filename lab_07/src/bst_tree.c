#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tree.h"
#include "bst_tree.h"



static bst_tree_node_t *get_successor(bst_tree_node_t *curr) {
    curr = curr->right;
    while (curr->left != NULL)
        curr = curr->left;
    return curr;
}

static bool bst_tree_remove(bst_tree_node_t **tree, reserved_word_t word, int *n_comps)
{
    bst_tree_node_t *temp;
    if (*tree == NULL)
        return false;
    ++*n_comps;
    int diff = strcmp((*tree)->reserved_word_info.word, word);
    if (diff > 0)
        return bst_tree_remove(&(*tree)->left, word, n_comps);
    if (diff < 0)
        return bst_tree_remove(&(*tree)->right, word, n_comps);
    
    if ((*tree)->left == NULL) 
    {
        temp = (*tree)->right;
        free((*tree)->reserved_word_info.help);
        free(*tree);
        *tree = temp;
        return true;
    }
    if ((*tree)->right == NULL) 
    {
        temp = (*tree)->left;
        free((*tree)->reserved_word_info.help);
        free(*tree);
        *tree = temp;
        return true;
    }

    temp = get_successor(*tree);
    free((*tree)->reserved_word_info.help);
    (*tree)->reserved_word_info = temp->reserved_word_info;
    (*tree)->reserved_word_info.help = strdup(temp->reserved_word_info.help);
    bst_tree_remove(&(*tree)->right, temp->reserved_word_info.word, n_comps);
    return tree;
}

bool abs_bst_tree_remove(void *tree, reserved_word_t word, int *n_comps)
{
    bst_tree_t *bst_tree = tree;
    return bst_tree_remove(&bst_tree->root, word, n_comps);
}

static bst_tree_node_t *create_node(reserved_word_info_t *word_info)
{
    bst_tree_node_t *node = malloc(sizeof(bst_tree_node_t));
    if (!node)
        return NULL;
    node->right = NULL;
    node->left = NULL;
    node->reserved_word_info = *word_info;
    return node;
}

static bool bst_tree_insert(bst_tree_node_t **tree, reserved_word_info_t *word_info, int *n_comps)
{
    int diff;

    if (*tree == NULL)
    {
        *tree = create_node(word_info);
        if (*tree == NULL)
            return false;
        return true;
    }

    diff = strcmp(word_info->word, (*tree)->reserved_word_info.word);
    ++*n_comps;
    
    if (diff < 0)
        return bst_tree_insert(&(*tree)->left, word_info, n_comps);
    if (diff > 0)
        return bst_tree_insert(&(*tree)->right, word_info, n_comps);
    return false;
}

bool abs_bst_tree_insert(void *tree, reserved_word_info_t *word_info, int *n_comps)
{
    bst_tree_t *bst_tree = tree;
    return bst_tree_insert(&bst_tree->root, word_info, n_comps);
}


char *abs_bst_tree_find(void *tree, reserved_word_t word, int *n_comps)
{
    int diff;
    bst_tree_t *bst_tree = tree;
    bst_tree_node_t *node = bst_tree->root;
    
    while (node != NULL)
    {
        diff = strcmp(word, node->reserved_word_info.word);
        ++*n_comps;
        if (diff == 0)
            return node->reserved_word_info.help;
        else if (diff < 0)
            node = node->left;
        else
            node = node->right;
    }
    return NULL;
}


static void bst_tree_print_dot_aux(FILE *f, bst_tree_node_t *node, int *null_count)
{
    if (!node->left && !node->right)
        return;
    
    if (node->left)
    {
        fprintf(f, "  %s -> %s;\n", node->reserved_word_info.word, node->left->reserved_word_info.word);
        bst_tree_print_dot_aux(f, node->left, null_count);
    }
    else
        tree_print_dot_null(f, node->reserved_word_info.word, ++(*null_count));

    if (node->right)
    {
        fprintf(f, "  %s -> %s;\n", node->reserved_word_info.word, node->right->reserved_word_info.word);
        bst_tree_print_dot_aux(f, node->right, null_count);
    }
    else
        tree_print_dot_null(f, node->reserved_word_info.word, ++(*null_count));
}

static void bst_tree_export_to_dot(FILE *f, bst_tree_node_t *root)
{
    int null_count = 0;
    fprintf(f, "digraph %s {\n", "bst_tree");
    fprintf(f, "  node [fontname=\"Arial\"];\n");

    if (!root)
        fprintf(f, "\n");
    else if (!root->right && !root->left)
        fprintf(f, "  %s;\n", root->reserved_word_info.word);
    else
        bst_tree_print_dot_aux(f, root, &null_count);

    fprintf(f, "}\n");
}

void abs_bst_tree_print(void *tree)
{
    bst_tree_t *bst_tree = tree;

    FILE *file = fopen("./graph.gv", "w");
    bst_tree_export_to_dot(file, bst_tree->root);
    fclose(file);

    system("dot -Tpng -o \"bst_tree.png\" ./graph.gv");
    system("open \"bst_tree.png\"");
}


static void bst_tree_free(bst_tree_node_t *tree)
{
    if (tree == NULL)
        return;

    bst_tree_free(tree->left);
    bst_tree_free(tree->right);
    free(tree->reserved_word_info.help);
    free(tree);
}

void abs_bst_tree_free(void *tree)
{
    bst_tree_t **bst_tree = tree;

    bst_tree_free((*bst_tree)->root);
    free(*bst_tree);
    *bst_tree = NULL;
}

void init_bst_tree(assoc_array_t *assoc_array)
{
    if (assoc_array->data != NULL)
        assoc_array->free(&assoc_array->data);
    
    bst_tree_t *bst_tree = malloc(sizeof(bst_tree_t));
    bst_tree->root = NULL;
    assoc_array->data = bst_tree;
    
    assoc_array->find = abs_bst_tree_find;
    assoc_array->insert = abs_bst_tree_insert;
    assoc_array->remove = abs_bst_tree_remove;
    assoc_array->clean_and_set_size = tree_set_size_stub;
    assoc_array->restruct = tree_restruct_stub;
    assoc_array->print = abs_bst_tree_print;
    assoc_array->set_comps = abs_tree_set_comps_stub;
    assoc_array->free = abs_bst_tree_free;
}

