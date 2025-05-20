#include "jslib/memory.h"
#include <sys/mman.h>
#include <errno.h>
#include <assert.h>

#include "memory_internal.h"

/**
 * TODO: Keep a separate list of free blocks of memory? Will it make searching faster? 
 */

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
static int insertBlock(Metadata* block) {

    if (block == NULL) return -1;

    Metadata* temp = data;

    while (temp!= NULL) {
        temp = temp->next;
    }

    temp = block;

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
    } 

    // 3. Create Metadata
    Metadata* metadata = (Metadata*)memory;
    metadata->size = alignedSize;
    metadata->next = NULL;
    metadata->free = 0;

    insertBlock(metadata);

    // 4. Return allocated memory block
    return metadata + 1;
}