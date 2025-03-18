#ifndef STRUCT_H__
#define STRUCT_H__

#define MATRIX_ROWS 1000
#define MATRIX_COLS 1000

#define FILENAME_LEN 30

typedef enum
{
    EXIT = 1,
    READ_FILE,
    READ_USER_STANDARD,
    READ_USER_COORD,
    FILL_WITH_SPARSITY,
    PRINT_STANDARD,
    PRINT_SPARSE,
    SUM_STANDARD_OUT_ST,
    SUM_STANDARD_OUT_SP,
    SUM_SPARSE_OUT_ST,
    SUM_SPARSE_OUT_SP,
    RUN_ANALYSIS
} menu_item_t;

typedef struct
{
    int matrix[MATRIX_ROWS][MATRIX_COLS];
    int rows;
    int cols;
} standard_matrix_t;

typedef struct
{
    int a[MATRIX_ROWS * MATRIX_COLS];
    int ia[MATRIX_ROWS * MATRIX_COLS];
    int ja[MATRIX_COLS];
    int rows;
    int cols;
    int elements_num;
} sparse_matrix_t;

typedef struct
{
    standard_matrix_t *standard_matrix;
    sparse_matrix_t *sparse_matrix;
} both_matrix_t;


#endif // #ifndef STRUCT_H__
