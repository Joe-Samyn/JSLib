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

/** ************ Internal Functions ************* */


/** ************ External Functions ************* */
int buddyInitGlobal(size_t maxOrder) {

    // TODO (joe): Check if globalBuddyAllocator has already been initialized
    // For now, we are going to always assume it hasn't been initialized
    int allocatorSize = power(2, maxOrder);
    size_t pageSize = sysconf(_SC_PAGE_SIZE);
    int size = align(allocatorSize, pageSize);

    void* memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    
    if (memory == MAP_FAILED)
    {
        // TODO (joe): Handle failure gracefully
        int error = errno;
        return ERROR;
    }

    // Convert the mapped memory region to a header struct to store metadata about the 
    // newly obtained block of memory
    struct Header* header = (struct Header*)memory;
    header->size = allocatorSize;
    header->free = TRUE;
    header->next = NULL;

    // Initialize the global allocator with the starting address of the newly mapped memory region
    globalBuddyAllocator.root = header;

    // Initialization was successful if we reach this point
    return SUCCESS;
} 

