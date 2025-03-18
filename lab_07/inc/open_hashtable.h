#ifndef OPEN_HASHTABLE_H__
#define OPEN_HASHTABLE_H__

#include "struct.h"


void init_open_hashtable(assoc_array_t *assoc_array);

void open_hashtable_restruct(open_hashtable_t *open_hashtable, int new_size);

#endif // #ifndef OPEN_HASHTABLE_H__
