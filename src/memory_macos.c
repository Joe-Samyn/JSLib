#include "jslib/memory.h"
#include "jslib/string.h"
#include <sys/mman.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>

#include "memory_internal.h"
#include "type.h"

MemoryPool globalPool = { };

/**
 * TODO: Keep a separate list of free blocks of memory? Will it make searching faster? 
 * TODO: Could verify memory pool is accurate before returing to ensure memory integrity. This might be worth it because the entire programs memory is stored in this memory pool.
 */
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

/**
 * Inserts a block of memory into the memory block list. 
 * 
 * The block of memory is always inserted at the end of the linked list. 
 * TODO: Not optimal and should be improved. 
 * 
 * @param block The block of memory to insert. Parameter cannot be NULL. If parameter is NULL, -1 is returned from function. 
 * @return 0 if insertion was successful. If insertion failed, a value less than 0 is returned. 
 */
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
     * TODO: Need to error out if max memory size allowed to be requested is exceeded
     */
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
            switch(errno) {
                case EINVAL: // len not greater than 0
                {
                } break;
                case EMFILE: // limit on mapped regions per process is exceeded
                {
                } break;
                case ENOMEM: // insufficient memory available 
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

/**
 * TODO: Need error handling
 * TODO: Need to ensure that pointer math lands on Metadata header, if not, then user did not pass valid pointer
 * TODO: Need to add memory coalescing to merge adjacent free blocks back together on free
 * TODO: Need to validate that ptr actually lies within memory pool and is not outside the bounds 
 */
void deallocMemory(void* ptr) {
    // Convert to byte for pointer arithmatic 
    Metadata* temp = (Metadata*)ptr;

    // Subtract bytes to get to start of Metadata header
    temp -= 1;

    // Free block of memory
    temp->free = TRUE;
}