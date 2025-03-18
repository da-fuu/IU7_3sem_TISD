#include "struct.h"
#include "hashtable.h"


int hash_function(char *word, int size)
{
    unsigned long long hash = 5381;
    
    while (*word != '\0')
    {
        hash = hash * 33 + *word;
        word++;
    }
    
    return (int)(hash % (unsigned long long)size);
}

bool is_prime(int num)
{
    for (int i = 2; i <= num / 2; i++)
        if (num % i == 0)
            return false;
    return true;
}

int find_next_size(int size)
{
    size = (int)(size * 1.5);
    if (size < INITIAL_SIZE)
        size = INITIAL_SIZE;

    while (!is_prime(size))
        size++;
    return size;
}
