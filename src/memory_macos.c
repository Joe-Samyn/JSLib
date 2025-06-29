#include "jslib/memory.h"
#include "jslib/string.h"
#include <sys/mman.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>

#include "memory_internal.h"
#include "type.h"

/**
 * TODO: 
 *  - deallocMemory needs memory coalescing to prevent the memory pool from getting fragmented and unusable 
 *  - deallocMemory needs to ensure pointer being freed lies within the bounds of a memory block 
 *  - deallocMemory needs to check if pointer being passed in is NULL, a NULL pointer cannot be freed
 *  - globalPool needs two lists, a list of allocated memory and a freelist to keep track of free memory regions. This will improve performance
 *  - allocMemory needs to hanle errors properly, currently it does nothing when errors are thrown from mmap
 *  - allocMemory and deallocMemory do not support multithreading/asynchronous programming
 *  - (Maybe) joinRegion may need to start with copies of A and B to ensure it can revert if error occurs
 */

MemoryPool globalPool = { };

/** ************************************************************************************************************************************ */
/** Internal Functions **/
/** ************************************************************************************************************************************ */

Metadata* splitRegion(Metadata* region, size_t size) {
    // 1. Verify region != NULL
    if (region == NULL) {
        return NULL;
    }

    // 2. Vefify region is large enough for aligned memory size
    int regOneSize = align(METADATA_SIZE + size, ALIGNMENT);
    if (region->size < regOneSize) {
        return NULL;
    }

    // 3a. Create temp pointer to region as type of byte 
    byte* temp = (byte*)region;

    // 3b. Add METADATA_SIZE + size to pointer to get start of next memory region 
    temp += regOneSize; // NOTE: This should jump forward (METADATA_SIZE + size) bytes

    // 3c. Add Metadata at this address that indicates start of next memory region 
    Metadata* regionTwo = (Metadata*)temp;
    regionTwo->size = region->size - regOneSize;
    regionTwo->free = 1;
    regionTwo->next = region->next;
    
    // 4. second->next = first->next & first->next = second
    region->next = regionTwo;
    region->free = 1;
    region->size = size;    

    // 6. return first region 
    return region;
}

Metadata* joinRegion(Metadata* a, Metadata* b) {
    if (a == NULL || b == NULL)
    {
        return NULL;
    }

    a->size += (b->size + METADATA_SIZE);
    a->next = b->next;
    return a;
}

Metadata* search(size_t size) {

    if (size < 1) {
        return NULL;
    }

    Metadata* block = NULL;
    Metadata* temp = NULL;
    temp = globalPool.root;
    int bestFit = INT32_MAX;
    while(temp!= NULL) {

        // Check for free blocks that match criteria
        if (temp->free && temp->size >= size) {
            int delta = temp->size - size;

            // We found a block that is a better fit
            if (bestFit > delta) {
                bestFit = delta;
                block = temp;
            }
        }

        temp = temp->next;
    }

    return block;
}

int insertBlock(Metadata* block) {

    if (block == NULL) return -1;

    if (!globalPool.root) {
        globalPool.root = block;
        return 0;
    }

    Metadata* temp = NULL;
    temp = globalPool.root;

    while (temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = block;

    return 0;
}

/** ************************************************************************************************************************************ */
/** External Functions **/
/** ************************************************************************************************************************************ */

void* allocMemory(size_t size) {

    if (size < 1) {
        return NULL;
    }

    // 1. Determine the proper size to allocate by aligning size to ALIGNMENT boundaries 
    size_t memorySize = size + sizeof(Metadata);
    size_t alignedSize = align(memorySize, ALIGNMENT);

    // 2. Search for open block or Create Block
    
    // 2a. Search 
    // We do not need to search for memorySize here, just size. If a block is in the pool, the sizeof(Metadata) is already accounted for
    Metadata* memory = search(size);

    // 2b. Split needed? What is a good buffer? 
    if (memory != NULL)
    {
        if ((memory->size - size) > MIN_BLOCK_SIZE)
        {
            memory = splitRegion(memory, size);
        }
    }
    else // 2c. Allocate new memory region
    {   
        // Round up to page size
        alignedSize = align(alignedSize, sysconf(_SC_PAGE_SIZE));
        memory = mmap(NULL, alignedSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (memory == MAP_FAILED) 
        {
            /**
             * TODO: Need to set error code that user can access
             */
            switch(errno) {
                case EINVAL: // len not greater than 0
                {
                } break;
                case EMFILE: // limit on mapped regions per process is exceeded
                {
                } break;
                case ENOMEM: // insufficient memory available or exceeded resource limit
                {
                } break;
            }

            return NULL;
        } 

        // 3. Create Metadata
        memory = (Metadata*)memory;
        memory->size = alignedSize;
        memory->next = NULL;
        memory->free = 1;

        // Split? 
        if (alignedSize - size > MIN_BLOCK_SIZE)
        {
            memory = splitRegion(memory, size);
        }

        // Block still needs to be inserted since we allocated a new page of memory from the OS
        insertBlock(memory);
    }

    // 4. Return allocated memory block
    memory->free = 0;
    void* usrMemory = (void*)(memory + 1);
    return usrMemory;
}

void deallocMemory(void* ptr) {
    // Convert to byte for pointer arithmatic 
    Metadata* temp = (Metadata*)ptr;

    // Subtract bytes to get to start of Metadata header
    temp -= 1;

    // Free block of memory
    temp->free = TRUE;

    // Check if next block is also free, if so combine into one single free region
    if (temp->next->free == TRUE)
    {
        joinRegion(temp, temp->next);
    }
}