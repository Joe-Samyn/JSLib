#ifndef MEMORYINTERNAL_H
#define MEMORYINTERNAL_H

#include <stdlib.h>
#include <uuid/uuid.h>


/** 
 * Number of bytes to align to when requesting memory from OS
 * TODO: This should be looked at further b/c not every machine will align to 8 bytes 
 * TODO: Calling a system call in a #define is not performant. Calls to system are expensive. This will place this
 * system call everywhere ALIGNMENT is used, which is bad. We need to rethink this. 
*/
/**
 * Define alignment size for system. This ignores SIMD for now. 
 * On most 64 bit systems, 8 byte alignment is used
 * On most 32 bit systems, 4 byte alignment is used
 */
#if defined(__aarch64__)
#define ALIGNMENT 8
#elif defined(__x86_64__)
#define ALIGNMENT 8
#else
#define ALIGNMENT 4
#endif

// Size of the metadata block
#define METADATA_SIZE sizeof(Metadata)
#define MIN_BLOCK_SIZE (METADATA_SIZE + ALIGNMENT)
/**
 * Aligns a number x to the next multiple of ALIGNMENT. Used to round up and get the 
 * largest memory size that is a multiple of ALIGNMENT needed to store the data.
 */
#define align(x, y) ((x + (y - 1) & ~(y - 1)))

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


/**
 * Search the `data` memory list for a block that best fits the size requirement.
 * 
 * The search function uses a best fit algorithm to find a size that is the closest to the desired size. The 
 * memory block will always be greater than or equal to the desired size. It will never be smaller than the desired size. 
 * 
 * @param size The size of memory required in bytes. Size must be greater than 0. 
 * @return A pointer to the start of the memory block satisfies the size criteria. If a memory block that matches the size criteria
 * cannot be found, NULL is returned. 
 */
/**
 * TODO: This is a naive implementation and can be improved
 */
Metadata* search(size_t size);

/**
 * Inserts a block of memory into the memory block list. 
 * 
 * The block of memory is always inserted at the end of the linked list. 
 * TODO: Not optimal and should be improved. 
 * 
 * @param block The block of memory to insert. Parameter cannot be NULL. If parameter is NULL, -1 is returned from function. 
 * @return 0 if insertion was successful. If insertion failed, a value less than 0 is returned. 
 */
int insertBlock(Metadata* block);

/**
 * Splits a memory region based on the desired size. 
 * 
 * This function takes a memory region and splits it into two separate regions: 
 * - First Region: sizeof(Metadata) + size
 * - Second Region: remaining bytes from original region 
 * 
 * This function will automatically handle: 
 * - Metadata initialization for both regions
 * - Linking both memory regions back into the memory pool
 * - Maintain alignment requirements
 * 
 * @note both resulting regions are marked as free after split. 
 * 
 * @param region The memory region to split. Must be: 
 *  - Large enough to hold desired size 
 *  - Not NULL
 * @param size The size in bytes of one of the regions that should result from the split. 
 *  - Greater than 0
 *  - Less than region->size 
 *  - Will be aligned to proper alignment 
 * @return On success: Pointer to first region 
 *         On failure: NULL and original region is preserved in pool. 
 */
Metadata* splitRegion(Metadata* region, size_t size);

/**
 * Get the root of the memory pool
 * @note This should only be used for unit tests. It's primary purpose is to get the 
 * root for verifying memory operations
 * @return Root node of the memory pool 
 */
Metadata* getMemPoolRoot();

/**
 * Clear the memory pool structures completely from memory to reset allocator
 * @note Should be used for unit tests only. This helps reset state between tests
 * to ensure there are no lingering pointers. 
 */
void clearMemoryPool();

#endif