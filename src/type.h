#ifndef TYPE_H
#define TYPE_H

#include <unistd.h>

#define byte char

#define TRUE 1
#define FALSE 0 

/**
 * This file contains all memory related operations such as copying memory, allocating memory,
 * duplicating memory, freeing memory, etc. 
 */
typedef struct Metadata {
    /**
     * Pointer to the next block of memory
     */
    struct Metadata* next;
    /**
     * Size in bytes of the memory region 
     */
    size_t size;
    /**
     * Flag indicating if memory is free or in use
     */
    int free;
} Metadata;

#endif