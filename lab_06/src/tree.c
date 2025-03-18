#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tree.h"

typedef int (*cmp_t)(const file_t *, const file_t *);


static int compare_names(const file_t *file_1, const file_t *file_2)
{
    if (file_1->name == file_2->name)
        return 0;
    return strcmp(file_1->name, file_2->name);
}

static int compare_dates(const file_t *file_1, const file_t *file_2)
{
    if (file_1->date.year != file_2->date.year)
        return file_1->date.year - file_2->date.year;
    if (file_1->date.month != file_2->date.month)
        return file_1->date.month - file_2->date.month;
    return file_1->date.day - file_2->date.day;
}

static int bst_tree_insert(tree_node_t **tree, tree_node_t *node, cmp_t comparator)
{
    int diff;

    if (*tree == NULL)
    {
        *tree = node;
        return OK;
    }

    diff = comparator(&node->file, &(*tree)->file);
    if (diff == 0)
    {
        return ERR_ELEM_ALREADY;
    }
    else if (diff < 0)
    {
        return bst_tree_insert(&(*tree)->left, node, comparator);
    }
    else
    {
        return bst_tree_insert(&(*tree)->right, node, comparator);
    }
}

int universal_tree_insert(tree_t *tree, tree_node_t *node)
{
    if (tree->sorted_by_date)
        return bst_tree_insert(&tree->root, node, compare_dates);
    return bst_tree_insert(&tree->root, node, compare_names);
}

static tree_node_t *bin_tree_find(tree_node_t *tree, const char *name)
{
    tree_node_t *found;

    if (strcmp(tree->file.name, name) == 0)
        return tree;
    if (tree->left)
    {
        found = bin_tree_find(tree->left, name);
        if (found != NULL)
            return found;
    }
    if (tree->right)
    {
        found = bin_tree_find(tree->right, name);
        if (found != NULL)
            return found;
    }
    return NULL;
}

int universal_tree_find(tree_t tree, const char *name, tree_node_t **found)
{
    int diff;
    tree_node_t *node = tree.root;
    tree_node_t *out;
    
    if (!tree.sorted_by_date)
    {
        while (node != NULL)
        {
            diff = strcmp(name, node->file.name);

            if (diff == 0)
            {
                *found = node;
                return OK;
            }
            else if (diff < 0)
                node = node->left;
            else
                node = node->right;
        }
    }
    else
    {
        out = bin_tree_find(node, name);
        if (out)
        {
            *found = out;
            return OK;
        }
    }
    return ERR_NOT_FOUND;
}


static tree_node_t *get_successor(tree_node_t *curr) {
    curr = curr->right;
    while (curr->left != NULL)
        curr = curr->left;
    return curr;
}

static tree_node_t *bst_tree_delete(tree_node_t *tree, tree_node_t *node, cmp_t comparator)
{
    tree_node_t *temp;
    int diff = comparator(&tree->file, &node->file);
    if (diff > 0)
        tree->left = bst_tree_delete(tree->left, node, comparator);
    else if (diff < 0)
        tree->right = bst_tree_delete(tree->right, node, comparator);
    else {
        if (tree->left == NULL) {
            temp = tree->right;
            free(tree->file.name);
            free(tree);
            return temp;
        }
        if (tree->right == NULL) {
            temp = tree->left;
            free(tree->file.name);
            free(tree);
            return temp;
        }

        temp = get_successor(tree);
        free(tree->file.name);
        tree->file = temp->file;
        tree->file.name = strdup(temp->file.name);
        tree->right = bst_tree_delete(tree->right, temp, comparator);
    }
    return tree;
}

int universal_tree_delete(tree_t *tree, const char *name)
{
    tree_node_t *found;
    int rc = universal_tree_find(*tree, name, &found);
    if (rc != OK)
        return rc;
    if (tree->sorted_by_date)
        tree->root = bst_tree_delete(tree->root, found, compare_dates);
    else
        tree->root = bst_tree_delete(tree->root, found, compare_names);
    return OK;
}


void print_file(file_t *file)
{
    printf("Название: %s\n", file->name);
    printf("Дата обращения: %.2hd.%.2hd.%.4d\n", file->date.day, file->date.month, file->date.year);
    printf("Файл ");
    if (!file->hidden)
        printf("не ");
    printf("скрытый\n");
    printf("Файл ");
    if (!file->system)
        printf("не ");
    printf("системный\n");
    printf("\n");
}

void bin_tree_print_pre(tree_node_t *tree)
{
    if (tree == NULL)
        return;
    print_file(&tree->file);
    bin_tree_print_pre(tree->left);
    bin_tree_print_pre(tree->right);
}

void bin_tree_print_in(tree_node_t *tree)
{
    if (tree == NULL)
        return;
    bin_tree_print_in(tree->left);
    print_file(&tree->file);
    bin_tree_print_in(tree->right);
}

void bin_tree_print_post(tree_node_t *tree)
{
    if (tree == NULL)
        return;
    bin_tree_print_post(tree->left);
    bin_tree_print_post(tree->right);
    print_file(&tree->file);
}


void bin_tree_free(tree_node_t **tree, bool all)
{
    if (*tree == NULL)
        return;

    bin_tree_free(&(*tree)->left, all);
    bin_tree_free(&(*tree)->right, all);
    if (all)
        free((*tree)->file.name);
    free(*tree);
    *tree = NULL;
}


void print_node(FILE *f, file_t *node)
{
    fprintf(f, " \"%s\\n%.2hd.%.2hd.%.4d\"", node->name, node->date.day, node->date.month, node->date.year);
}


static void bin_tree_print_dot_null(FILE *f, file_t *name, int null_count)
{
    fprintf(f, "  null%d [shape=point];\n", null_count);
    fprintf(f, "  ");
    print_node(f, name);
    fprintf(f, " -> null%d;\n", null_count);
}

static void bin_tree_print_dot_aux(FILE *f, tree_node_t *node, int *null_count)
{
    if (!node->left && !node->right)
        return;
    
    if (node->left)
    {
        fprintf(f, "  ");
        print_node(f, &node->file);
        fprintf(f, " -> ");
        print_node(f, &node->left->file);
        fprintf(f, ";\n");
        bin_tree_print_dot_aux(f, node->left, null_count);
    }
    else
        bin_tree_print_dot_null(f, &node->file, ++(*null_count));

    if (node->right)
    {
        fprintf(f, "  ");
        print_node(f, &node->file);
        fprintf(f, " -> ");
        print_node(f, &node->right->file);
        fprintf(f, ";\n");
        bin_tree_print_dot_aux(f, node->right, null_count);
    }
    else
        bin_tree_print_dot_null(f, &node->file,++(*null_count));
}

void bin_tree_export_to_dot(FILE *f, const char *tree_name, tree_node_t *root)
{
    int null_count = 0;
    fprintf(f, "digraph %s {\n", tree_name);
    fprintf(f, "  node [fontname=\"Arial\"];\n");

    if (!root)
        fprintf(f, "\n");
    else if (!root->right && !root->left)
    {
        fprintf(f, "  ");
        print_node(f, &root->file);
        fprintf(f, ";\n");
    }
    else
        bin_tree_print_dot_aux(f, root, &null_count);

    fprintf(f, "}\n");
}

tree_node_t *wrong_bst_del_old_files(tree_node_t *tree, file_t *sample)
{
    if (tree->left)
        tree->left = wrong_bst_del_old_files(tree->left, sample);
    if (tree->right)
        tree->right = wrong_bst_del_old_files(tree->right, sample);
    if (compare_dates(&tree->file, sample) < 0)
        return bst_tree_delete(tree, tree, compare_names);
    return tree;
}

tree_node_t *correct_bst_del_old_files(tree_node_t *tree, file_t *sample)
{
    int diff = compare_dates(&tree->file, sample);
    
    if (tree->left)
        tree->left = correct_bst_del_old_files(tree->left, sample);
    if (tree->right && diff < 0)
        tree->right = correct_bst_del_old_files(tree->right, sample);
    if (diff < 0)
        return bst_tree_delete(tree, tree, compare_dates);
    return tree;
}

void universal_del_old_files(tree_t *tree, file_t *sample)
{
    if (!tree->sorted_by_date)
        tree->root = wrong_bst_del_old_files(tree->root, sample);
    else
        tree->root = correct_bst_del_old_files(tree->root, sample);
}
