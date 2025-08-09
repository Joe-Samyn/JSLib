#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <stddef.h>
#include "type.h"

/* Error Codes */
/**
 * No error currently exists 
 */
#define NO_ERR 0

/**
 * Allocator was previously initialized and cannot be reinitialized
 */
#define PREV_INIT 1

/**
 * An argument passed to the function was invalid
 */
#define INVAL_ARG 2

/**
 * An internal error occurred when trying to call OS level functions like mmap, sbrk, etc.
 */
#define OS_ERR 3

/**
 * The allocator is out of available memory.
 */
#define NO_MEM 4


/** 
    TODO: Add errorCode property that is set when error occurs. Maybe make it a queue and pop the most recent error code off the queue? 
*/
/**
 * An object that represents a memory pool. 
 */
struct BuddyAllocator {
    /**
     * Root node of the memory pool
     */
    byte *start;
    /**
     * The end address of the memory region
     */
    byte *end;
    /**
     * Order of the entire memory block managed by this allocator
     */
    unsigned int order;
    /**
     * Flag indicating if BuddyAllocator has been intialized previously
     */
    unsigned int initialized;

};

/**
 * A pointer to the head of the list of memory blocks
 * TODO: Maybe look into a memory pool system that can support multiple allocators such as MemoryPool object
 */
extern struct BuddyAllocator globalBuddyAllocator;

extern int errorCode;

/* =============== Generic Memory Operations ======================= */
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

/* ================ Buddy Allocator Interface ================== */
/**
 * Initializes a global buddy allocator that manages 2^(maxOrder) bytes of memory. The result of 2^(maxOrder) is aligned (rounded up) to the 
 * next largest page size. This ensures memory is requested from the OS in page multiples. The global allocator
 * object is entirely managed by the JSLib library and is meant to be used for simple programs. Custom buddy 
 * allocators are created using the `buddyInitCustom` function. 
 * 
 * @note Currently the Global Buddy Allocator is not thread safe, meaning there is no protection against accessing the allocator
 * from multiple threads simultaneously. Thread safety will be coming in a future update.
 * 
 * @param maxOrder The maximum order of memory this buddy allocator will manage. The resulting size is proportional to size
 * 2 to the order of maxOrder (i.e. 2^(maxOrder))
 * @return SUCCESS is returned when initialization is successful. ERROR is returned when an error occurred during initialization 
 * and a global buddy allocator could not be created. 
 */
int buddyInitGlobal(size_t maxOrder);

/**
 * Allocate a block that is greater than or equal to the `requestedSize`. The allocator searches the list of free blocks 
 * for smallest block that can satisfy the request (i.e. >= requestedSize). Larger memory blocks are continually divided in half until the closest 
 * size block that is greater than `requestedSize` is achieved. The full allocation algorithm can be reviewed here: <add link>
 * 
 * @note Currently the Global Buddy Allocator is not thread safe, meaning there is no protection against accessing the allocator
 * from multiple threads simultaneously. Thread safety will be coming in a future update.
 * 
 * @param requestSize The size of memory to allocate. Size will be rounded up to the nearest power of 2 if it is not a power of 2. 
 * @return A pointer to the start of the memory region is returned on successful allocation. NULL is returned 
 * if allocation fails. In this case, the global errorCode variable is set to indicate the failure reason.
 */
void* buddyAlloc(size_t requestedSize);

/**
 * Deallocates the block of memory referenced by `ptr`. During deallocation, if an adjecent memory block is also free, the two blocks (the block ptr resides in
 * and the free adjacent block) are joined into a single block of free memory. If `ptr` does not point to a valid block of memory that was allocated by the global
 * buddy allocator, then no deallocations occur and the block of memory is untouched. 
 * 
 * @param ptr A pointer to the block of memory to be deallocated. A ptr must point to a valid block of memory that was allocated by the global buddy allocator
 * and it cannot be NULL. 
 */
void buddyFree(void* ptr);

#endif