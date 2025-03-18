#ifndef TREE_H__
#define TREE_H__

#include <stdbool.h>
#include <stdio.h>

#define OK 0
#define ERR_NOT_FOUND (-1)
#define ERR_ELEM_ALREADY (-2)


typedef struct
{
    short day;
    short month;
    int year;
} date_t;

typedef struct
{
    char *name;
    date_t date;
    bool hidden;
    bool system;
} file_t;

typedef struct tree_node
{
    file_t file;
    struct tree_node *left;
    struct tree_node *right;
} tree_node_t;

typedef struct
{
    tree_node_t *root;
    bool sorted_by_date;
} tree_t;


int universal_tree_insert(tree_t *tree, tree_node_t *node);

int universal_tree_find(tree_t tree, const char *name, tree_node_t **found);

int universal_tree_delete(tree_t *tree, const char *name);

void print_file(file_t *file);

void bin_tree_print_pre(tree_node_t *tree);

void bin_tree_print_in(tree_node_t *tree);

void bin_tree_print_post(tree_node_t *tree);

void bin_tree_free(tree_node_t **tree, bool all);

void bin_tree_export_to_dot(FILE *f, const char *tree_name, tree_node_t *root);

void universal_del_old_files(tree_t *tree, file_t *sample);

tree_node_t *wrong_bst_del_old_files(tree_node_t *tree, file_t *sample);

tree_node_t *correct_bst_del_old_files(tree_node_t *tree, file_t *sample);


#endif // #ifndef TREE_H__
