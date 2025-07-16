#include "jslib/memory.h"
#include "jslib/string.h"
#include "jslib/math.h"
#include <sys/mman.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

#include "memory_internal.h"
#include "type.h"


/**
 * TODO: 
 * 
 */


struct BuddyAllocator globalBuddyAllocator = { };
int errorCode = NO_ERR;

/** ************ Internal Functions ************* */

/**
 * Recursively splits a block of memory in half until the size of the block is at least `requestedSize`.
 * DRAFT: This function will recursively split the chunk until it gets a size in bytes that is at least `requestedSize`. The resulting size will be >= `requestedSize`. 
 * Continually dividing by two allows the memory to stay aligned to system requirements. The smallest size a chunk can be split down to is `requestedSize` + HEADER_SIZE
 * rounded up to the next power of 2. The two halves that result from a split are associated to each other as 'buddies', meaning the chunks point to each other. This allows 
 * us to look for potential regions to join back together and coalesce memory when memory is freed. 
 * 
 * @param chunk The chunk of memory to reduce down to the desired size by continually dividing it by two. 
 * @param requestedSize The desired size in bytes 
 * @returns A pointer to the memory block whose size in bytes is greater than or equal to `requestedSize`. If no block is found, NO_MEM is set as the global error code and NULL
 * is returned.
 */
static struct Header* split(struct Header* chunk, size_t requestedSize)
{
    // Recursion break condition
    if ((requestedSize + HEADER_SIZE) >= (chunk->size / 2))
    {
        return chunk;
    }

    unsigned int memorySize = chunk->size + HEADER_SIZE;
    void* memory = (void*)chunk;

    // Split the chunk in half
    unsigned int newMemorySize = memorySize / 2;
    void* newChunk = memory + newMemorySize;
    
    // Create new buddy node
    struct Header* buddy = (struct Header*)newChunk;
    buddy->size = newMemorySize - HEADER_SIZE;
    buddy->free = TRUE;
    buddy->next = chunk->next;
    buddy->prev = chunk;

    // Update existing chunk to new size and attach its buddy
    chunk->size = newMemorySize - HEADER_SIZE;
    chunk->next = buddy;

    return split(chunk, requestedSize);
}

/**
 * TODO: Clean up this documentation
 * Searches for a block of memory whose size is at least `requestedSize` in bytes (i.e. size >= requestedSize). The search algorithm
 * will perform repeated splits on blocks of memory if `requestedSize` is less than half the size of the best fit block (i.e. requestedSize <= block.size / 2).
 * The total size of the memory block found is equal to `requestedSize` + `HEADER_SIZE` to ensure there is enough space for header data and user data. 
 * 
 * @param requestedSize The desired size of the memory block.
 * @return A pointer to the header (or start) of the block of memory 
 */
static void* search(size_t requestedSize) {

    // requestedSize must be greater than 0 to get a valid memory region
    if (requestedSize <= 0) 
    {
        errorCode = INVAL_ARG;
        return NULL;
    }

    // Linearly search linked list for now. 
    // TODO: This is not a good solution. We need a freelist implementation, preferable using a self balancing tree to make 
    // free memory lookups fast: O(lg(n)) time
    struct Header* node = globalBuddyAllocator.root;
    struct Header* bestFit = NULL;
    while (node != NULL) 
    {
        // TODO: When a freelist is implemented, bestFit can be set to the root free node and the NULL check will not be required
        if (node->free)
        {
            // NOTE: This NULL check will go away once a freelist is implemented
            if (bestFit == NULL)
            {
                bestFit = node;
            }
            else if (node->size > requestedSize && node->size < bestFit->size)
            {
                bestFit = node;
            }
        }

        node = node->next;
    }

    if (bestFit == NULL)
    {   
        errorCode = NO_MEM;
        return NULL;
    }
    

    if (requestedSize < (bestFit->size / 2)) 
        bestFit = split(bestFit, requestedSize);

    return bestFit;
}


/** ************ External Functions ************* */
int buddyInitGlobal(size_t maxOrder) {

    // Check if global buddy allocator has already been initialized
    if (globalBuddyAllocator.root != 0x0) 
    {
        errorCode = PREV_INIT;
        return ERROR;
    }

    // Ensure maxOrder is valid before proceeding to mmap call
    if (maxOrder <= 0) 
    {
        errorCode = INVAL_ARG;
        return ERROR;
    }

    // We must add HEADER_SIZE here or the user may end up with less memory than they were expecting
    int allocatorSize = power(2, maxOrder) + HEADER_SIZE;
    size_t pageSize = sysconf(_SC_PAGE_SIZE);
    int size = align(allocatorSize, pageSize);

    // size cannot and should not be 0 here. If size is 0, there is a bug
    assert(size != 0);

    void* memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    
    // If we get here, something else failed that the user has no control over. Let them know an OS level error occurred with the library
    if (memory == MAP_FAILED)
    {
        errorCode = OS_ERR;
        return ERROR;
    }

    // Convert the mapped memory region to a header struct to store metadata about the 
    // newly obtained block of memory
    struct Header* header = (struct Header*)memory;
    header->size =  size - HEADER_SIZE;
    header->free = TRUE;
    header->next = NULL;
    header->order = log2(size);

    // Initialize the global allocator with the starting address of the newly mapped memory region
    globalBuddyAllocator.root = header;

    // Initialization was successful if we reach this point
    return SUCCESS;
} 

void* buddyAlloc(size_t requestedSize) {

    // TODO: We can probably use a ternary here and potentially a conditional data move which would result in better branch prediction 
    void* memory = search(requestedSize);
    return memory ? memory + HEADER_SIZE : memory;
}