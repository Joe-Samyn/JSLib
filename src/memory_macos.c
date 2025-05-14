#include "jslib/memory.h"
#include <sys/mman.h>
#include <errno.h>



/**
 * TODO:
 * 	- Determine if free memory exists and calling to system is required
 * 	- Split memory regions using a memory algorithm if free memory exists that is too large
 */
void* allocMemory(size_t size) {

    // 1. Determine the proper size to allocate
    int memorySize = size + sizeof(Metadata);
    int alignedSize = align(memorySize);

    // 2. Create Block
    void* memory = mmap(NULL, alignedSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if ((long)memory == -1) 
    {
        if (errno == EINVAL)
        {
           return NULL;
        }
    } 

    // 3. Create Metadata
    Metadata* metadata = (Metadata*)memory;
    metadata->size = size;
    metadata->next = NULL;
    metadata->free = 0;

    // 4. Allocate memory
    return metadata + 1;
}
