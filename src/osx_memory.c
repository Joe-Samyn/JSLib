#include "jslib/memory.h"
#include "jslib/string.h"
#include "jslib/math.h"
#include <sys/mman.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>

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
 * TODO: Clean up this documentation
 * Searches for a block of memory whose size is at least `requestedSize` in bytes (i.e. size >= requestedSize). The search algorithm
 * will perform repeated splits on blocks of memory if `requestedSize` is less than half the size of the best fit block (i.e. requestedSize <= block.size / 2).
 * The total size of the memory block found is equal to `requestedSize` + `HEADER_SIZE` to ensure there is enough space for header data and user data. 
 * 
 * @param requestedSize The desired size of the memory block.
 * @return A pointer to the header (or start) of the block of memory 
 */
void* search(size_t requestedSize) {

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
    
    // TODO: The node should then be split if it is too large to prevent waste of space

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
    header->size = size;
    header->free = TRUE;
    header->next = NULL;

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