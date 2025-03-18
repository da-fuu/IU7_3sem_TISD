#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tree.h"
#include "avl_tree.h"


int get_height(avl_tree_node_t *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

int b_factor(avl_tree_node_t *node)
{
    return get_height(node->right) - get_height(node->left);
}

void fix_height(avl_tree_node_t *node)
{
    if (get_height(node->left) > get_height(node->right))
        node->height = get_height(node->left) + 1;
    else
        node->height = get_height(node->right) + 1;
}

avl_tree_node_t *rotate_right(avl_tree_node_t *right_node)
{
    avl_tree_node_t *left_node = right_node->left;
    right_node->left = left_node->right;
    left_node->right = right_node;
    fix_height(right_node);
    fix_height(left_node);
    return left_node;
}

avl_tree_node_t *rotate_left(avl_tree_node_t *left_node)
{
    avl_tree_node_t *right_node = left_node->right;
    left_node->right = right_node->left;
    right_node->left = left_node;
    fix_height(left_node);
    fix_height(right_node);
    return right_node;
}

avl_tree_node_t *balance(avl_tree_node_t *node)
{
    fix_height(node);
    if (b_factor(node) == 2)
    {
        if (b_factor(node->right) < 0)
            node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    if (b_factor(node) == -2)
    {
        if (b_factor(node->left) > 0)
            node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    return node;
}

static avl_tree_node_t *create_node(reserved_word_info_t *word_info)
{
    avl_tree_node_t *node = malloc(sizeof(avl_tree_node_t));
    if (!node)
        return NULL;
    node->right = NULL;
    node->left = NULL;
    node->reserved_word_info = *word_info;
    node->height = 1;
    return node;
}

static bool avl_tree_insert(avl_tree_node_t **node, reserved_word_info_t *word_info, int *n_comps)
{
    int diff;
    
    if (*node == NULL)
    {
        *node = create_node(word_info);
        if (*node == NULL)
            return false;
        return true;
    }

    diff = strcmp(word_info->word, (*node)->reserved_word_info.word);
    ++*n_comps;
    
    if (diff < 0)
    {
        if (!avl_tree_insert(&(*node)->left, word_info, n_comps))
            return false;
    }
    else if (diff > 0)
    {
        if (!avl_tree_insert(&(*node)->right, word_info, n_comps))
            return false;
    }
    else
        return false;
    *node = balance(*node);
    return true;
}

bool abs_avl_tree_insert(void *tree, reserved_word_info_t *word_info, int *n_comps)
{
    avl_tree_t *avl_tree = tree;
    return avl_tree_insert(&avl_tree->root, word_info, n_comps);
}


avl_tree_node_t *find_min(avl_tree_node_t *node)
{
    while (node->left != NULL)
        node = node->left;
    
    return node;
}

avl_tree_node_t *remove_min(avl_tree_node_t *node)
{
    if (node->left == NULL)
        return node->right;
    node->left = remove_min(node->left);
    return balance(node);
}

static bool avl_tree_remove(avl_tree_node_t **node, reserved_word_t word, int *n_comps)
{
    avl_tree_node_t *min_node, *right_node, *left_node, *temp;
    if (*node == NULL) 
        return false;

    int diff = strcmp(word, (*node)->reserved_word_info.word);
    ++*n_comps;
    
    if (diff < 0)
    {
        if (!avl_tree_remove(&(*node)->left, word, n_comps))
            return false;
    }
    else if (diff > 0)
    {
        if (!avl_tree_remove(&(*node)->right, word, n_comps))
            return false;
    }
    else
    {
        left_node = (*node)->left;
        right_node = (*node)->right;
        if ((*node)->left == NULL)
        {
            temp = (*node)->right;
            free((*node)->reserved_word_info.help);
            free(*node);
            *node = temp;
            return true;
        }
        if ((*node)->right == NULL)
        {
            temp = (*node)->left;
            free((*node)->reserved_word_info.help);
            free(*node);
            *node = temp;
            return true;
        }

        free((*node)->reserved_word_info.help);
        free(*node);
        
        min_node = find_min(right_node);
        min_node->right = remove_min(right_node);
        min_node->left = left_node;
        *node = balance(min_node);
        return true;
    }
    *node = balance(*node);
    return true;
}

bool abs_avl_tree_remove(void *tree, reserved_word_t word, int *n_comps)
{
    avl_tree_t *avl_tree = tree;
    return avl_tree_remove(&avl_tree->root, word, n_comps);
}


char *abs_avl_tree_find(void *tree, reserved_word_t word, int *n_comps)
{
    int diff;
    avl_tree_t *avl_tree = tree;
    avl_tree_node_t *node = avl_tree->root;

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


static void avl_tree_print_dot_aux(FILE *f, avl_tree_node_t *node, int *null_count)
{
    if (!node->left && !node->right)
        return;

    if (node->left)
    {
        fprintf(f, "  %s -> %s;\n", node->reserved_word_info.word, node->left->reserved_word_info.word);
        avl_tree_print_dot_aux(f, node->left, null_count);
    }
    else
        tree_print_dot_null(f, node->reserved_word_info.word, ++(*null_count));

    if (node->right)
    {
        fprintf(f, "  %s -> %s;\n", node->reserved_word_info.word, node->right->reserved_word_info.word);
        avl_tree_print_dot_aux(f, node->right, null_count);
    }
    else
        tree_print_dot_null(f, node->reserved_word_info.word, ++(*null_count));
}

static void avl_tree_export_to_dot(FILE *f, avl_tree_node_t *root)
{
    int null_count = 0;
    fprintf(f, "digraph %s {\n", "avl_tree");
    fprintf(f, "  node [fontname=\"Arial\"];\n");

    if (!root)
        fprintf(f, "\n");
    else if (!root->right && !root->left)
        fprintf(f, "  %s;\n", root->reserved_word_info.word);
    else
        avl_tree_print_dot_aux(f, root, &null_count);

    fprintf(f, "}\n");
}

void abs_avl_tree_print(void *tree)
{
    avl_tree_t *avl_tree = tree;

    FILE *file = fopen("./graph.gv", "w");
    avl_tree_export_to_dot(file, avl_tree->root);
    fclose(file);

    system("dot -Tpng -o \"avl_tree.png\" ./graph.gv");
    system("open \"avl_tree.png\"");
}


static void avl_tree_free(avl_tree_node_t *tree)
{
    if (tree == NULL)
        return;

    avl_tree_free(tree->left);
    avl_tree_free(tree->right);
    free(tree->reserved_word_info.help);
    free(tree);
}

void abs_avl_tree_free(void *tree)
{
    avl_tree_t **avl_tree = tree;

    avl_tree_free((*avl_tree)->root);
    free(*avl_tree);
    *avl_tree = NULL;
}


void init_avl_tree(assoc_array_t *assoc_array)
{
    if (assoc_array->data != NULL)
        assoc_array->free(&assoc_array->data);
    
    avl_tree_t *avl_tree = malloc(sizeof(avl_tree_t));
    avl_tree->root = NULL;
    assoc_array->data = avl_tree;

    assoc_array->find = abs_avl_tree_find;
    assoc_array->insert = abs_avl_tree_insert;
    assoc_array->remove = abs_avl_tree_remove;
    assoc_array->print = abs_avl_tree_print;
    assoc_array->set_comps = abs_tree_set_comps_stub;
    assoc_array->clean_and_set_size = tree_set_size_stub;
    assoc_array->restruct = tree_restruct_stub;
    assoc_array->free = abs_avl_tree_free;
}
