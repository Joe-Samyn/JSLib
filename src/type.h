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
    int size;
    int free;
    struct Header* next;
};

#endif