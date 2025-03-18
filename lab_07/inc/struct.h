#ifndef STRUCT_H__
#define STRUCT_H__

#include <stdbool.h>
#include <stddef.h>

#define MAX_WORD_LEN 16

typedef char reserved_word_t[MAX_WORD_LEN];

typedef struct
{
    char *help;
    reserved_word_t word;
} reserved_word_info_t;


typedef struct bin_tree_node
{
    reserved_word_info_t reserved_word_info;
    struct bin_tree_node *left;
    struct bin_tree_node *right;
} bst_tree_node_t;

typedef struct
{
    bst_tree_node_t *root;
} bst_tree_t;


typedef struct avl_tree_node
{
    reserved_word_info_t reserved_word_info;
    int height;
    struct avl_tree_node *left;
    struct avl_tree_node *right;
} avl_tree_node_t;

typedef struct
{
    avl_tree_node_t *root;
} avl_tree_t;


typedef struct open_ht_node
{
    reserved_word_info_t reserved_word_info;
    struct open_ht_node *next;
} open_ht_node_t;

typedef struct
{
    open_ht_node_t **array;
    int size;
    int max_comparisons;
} open_hashtable_t;


typedef struct
{
    reserved_word_info_t reserved_word_info;
    enum
    {
        FREE,
        USED,
        DELETED
    } state;
} closed_ht_elem_t;

typedef struct
{
    closed_ht_elem_t *array;
    int size;
    int max_comparisons;
} closed_hashtable_t;


typedef struct
{
    void *data;
    bool (*insert)(void *data, reserved_word_info_t *word_info, int *n_comps);
    bool (*remove)(void *data, reserved_word_t word, int *n_comps);
    char *(*find)(void *data, reserved_word_t word, int *n_comps);
    void (*print)(void *data);
    bool (*set_comps)(void *data, int comps);
    void (*clean_and_set_size)(void *data, int new_size);
    void (*restruct)(void *data, int comps);
    void (*free)(void *data);
} assoc_array_t;


#endif // #ifndef STRUCT_H__
