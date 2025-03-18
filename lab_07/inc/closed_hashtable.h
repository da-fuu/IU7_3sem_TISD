#ifndef CLOSED_HASHTABLE_H__
#define CLOSED_HASHTABLE_H__

#include "struct.h"


void init_closed_hashtable(assoc_array_t *assoc_array);

void closed_hashtable_restruct(closed_hashtable_t *closed_hashtable, int new_size);


#endif // #ifndef CLOSED_HASHTABLE_H__
