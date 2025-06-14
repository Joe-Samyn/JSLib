#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

/** 
    TODO: Add errorCode property that is set when error occurs. Maybe make it a queue and pop the most recent error code off the queue? 
*/

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
void* memoryCopy(void* dest, const void* src, size_t n);

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
void* memorySet(void* dest, unsigned char value, size_t n);

/**
 * Allocate a block of memory that is at least `size` in bytes. Memory is uninitialized on allocation.
 * Memory returned is always aligned to the largest type on the platform. The alignment is determined
 * by the ALIGNMENT macro. 
 * 
 * The allocator will use the "best fit" algorithm to search for a memory block that matches the size
 * being requested. This is not the most optimal and could lead to higher fragmentation depending on how
 * the allocator is used. This is an initial MVP implementation and will be optiomized.
 * 
 * TODO: This current implementation is NOT thread safe.
 * TODO: This implementation will only work on OSX, it needs to be ported to Linux and Windows 
 * 
 * @param size The size in bytes of the required memory region. Size must be greater than 0. If
 * 0 is provided, a NULL pointer is returned. 
 * @return A pointer to the allocated memory block matching the size requirement. The returned 
 * block will be greater than or equal to the requested size. A NULL pointer is returned on 
 * failure to allocate memory. 
 */
void* allocMemory(size_t size);

void deallocMemory(void* ptr);

#endif