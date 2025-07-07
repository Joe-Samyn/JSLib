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
    header->size = allocatorSize;
    header->free = TRUE;
    header->next = NULL;

    // Initialize the global allocator with the starting address of the newly mapped memory region
    globalBuddyAllocator.root = header;

    // Initialization was successful if we reach this point
    return SUCCESS;
} 

