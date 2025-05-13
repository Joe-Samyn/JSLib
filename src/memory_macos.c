#include "jslib/memory.h"



/**
 * TODO:
 * 	- Determine if free memory exists and calling to system is required
 * 	- Split memory regions using a memory algorithm if free memory exists that is too large
 */
void* allocMemory(size_t size) {
    
    // 1. Determine the proper size to allocate
    Metadata* metadata;
    int memorySize = size + sizeof(Metadata);
    int alignedSize = align(memorySize);

	// 2. Create Metadata

	// 3. Create Block

	// 4. Allocate memory


    return NULL;
}
