#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

// Size of the metadata block
#define METADATA_SIZE sizeof(Metadata)

/** 
 * Number of bytes to align to when requesting memory from OS
 * TODO: This should be looked at further b/c not every machine will align to 8 bytes 
*/
#define ALIGNMENT 8

/**
 * Aligns a number x to the next multiple of ALIGNMENT. Used to round up and get the 
 * largest memory size that is a multiple of ALIGNMENT needed to store the data.
 */
#define align(x) ((x + (ALIGNMENT - 1) & ~(ALIGNMENT - 1)))

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
     * Size of the memory region 
     */
    size_t size;
    /**
     * Flag indicating if memory is free or in use
     */
    int free;
};

/**
 * Copies data from a source memory region into destination memory region
 * 
 * The memory addresses must not overlap. 
 * The pointers dest and src must not be null. 
 * 
 * @param dest A pointer to the memory region that is at least n bytes in size to copy data into 
 * @param src A pointer to the memory region that contains at least n bytes to copy data from 
 * @param n The number of bytes to copy from src into dest
 * @return A pointer to dest where data was copied.
 */
void* memcpy(void* dest, const void* src, size_t n);

/**
 * Sets n bytes of the block of memory pointed to by dest to the specified value
 * 
 * The dest pointer must be a valid pointer to writeable memory
 * 
 * @param dest A pointer to the memory region that is at least n bytes in size to be set
 * @param value The value to set n bytes of data to
 * @param n The number of bytes to set 
 * @return dest
 */
void* memset(void* dest, unsigned char value, size_t n);

/**
 * Allocate size bytes of uninitialized memory 
 * 
 * If size is zero, a null pointer will be returned.
 * TODO: This current implementation is NOT thread safe.
 * TODO: This implementation will only work on Linux, it needs to be ported to MacOS and Windows 
 * 
 * @param 
 */
void* alloc(size_t size);

void dealloc();

#endif