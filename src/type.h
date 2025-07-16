#ifndef TYPE_H
#define TYPE_H

#include <unistd.h>

#define byte char

#define TRUE 1
#define FALSE 0
#define ERROR -1
#define SUCCESS 1 

/**
 * Represents the header of a memory block. The header contains information about the block such as
 * size of the block, whether the block is free or not, and a pointer to the next contiguous block 
 * of memory.
 */
struct Header {
    /**
     * Size in bytes of the user space (excluding header)
     */
    unsigned int size;
    /**
     * A flag indicating whether the block of memory is free or allocated
     */
    unsigned int free;
    /**
     * Order of the entire memory chunk (not excluding header)
     */
    unsigned int order;

    /* optional and will be removed */
    struct Header* next;
    struct Header* prev;
};

#endif