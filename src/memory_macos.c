#include "jslib/memory.h"
#include <sys/mman.h>
#include <errno.h>
#include <assert.h>

/**
 * TODO: Keep a separate list of free blocks of memory? Will it make searching faster? 
 * TODO: Write unit tests for 
 */

// Size of the metadata block
#define METADATA_SIZE sizeof(Metadata)

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
     * Size in bytes of the memory region 
     */
    size_t size;
    /**
     * Flag indicating if memory is free or in use
     */
    int free;
} Metadata;


/**
 * A pointer to the head of the list of memory blocks
 */
static Metadata* data = NULL;

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
static Metadata* search(size_t size);

/**
 * TODO:
 * 	- Determine if free memory exists and calling to system is required
 * 	- Split memory regions using a memory algorithm if free memory exists that is too large
 */
void* allocMemory(size_t size) {

    if (size < 1) {
        return NULL;
    }

    // 1. Determine the proper size to allocate by aligning size to ALIGNMENT boundaries 
    /**
     * TODO: What happens if memorySize is too large? 
     */
    size_t alignedSize = align(size);
    assert(sizeof(Metadata) % ALIGNMENT == 0);
    size_t memorySize = alignedSize + sizeof(Metadata);

    // 2. Search for open block or Create Block
    
    // 2a. Search 

    // 2b. Split needed? 

    // 2c. Allocate new memory region
    void* memory = mmap(NULL, memorySize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    /**
     * TODO: Handle more error scenarios
     */
    if (memory == MAP_FAILED) 
    {
        if (errno == EINVAL)
        {
           return NULL;
        }
    } 

    // 3. Create Metadata
    /**
     * TODO: Add block into memory block list
     */
    Metadata* metadata = (Metadata*)memory;
    metadata->size = alignedSize;
    metadata->next = NULL;
    metadata->free = 0;

    // 4. Return allocated memory block
    return metadata + 1;
}

/**
 * TODO: This is a naive implementation and can be improved
 */
static Metadata* search(size_t size) {

    if (size < 1) {
        return NULL;
    }

    Metadata* block = NULL;
    Metadata* temp = data;
    int bestFit = INT32_MAX;
    while(temp != NULL) {

        // Check for free blocks that match criteria
        if (temp->free && temp->size >= size) {
            int delta = temp->size - size;

            // We found a block that is a better fit
            if (bestFit > delta) {
                block = temp;
            }
        }

        temp = temp->next;
    }
}