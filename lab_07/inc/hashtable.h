#ifndef HASHTABLE_H__
#define HASHTABLE_H__

#define MAX_COMPARISONS 3
#define INITIAL_SIZE 10

#include "struct.h"

int hash_function(char *word, int size);

int find_next_size(int size);


#endif // #ifndef HASHTABLE_H__
