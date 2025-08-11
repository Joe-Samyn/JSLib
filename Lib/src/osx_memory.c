#include "jslib/memory.h"
#include "jslib/string.h"
#include "jslib/math.h"
#include <sys/mman.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>

#include "memory_internal.h"
#include "type.h"
#include "math.h"

/**
 * TODO: Pointer arithmetic is only byte level with void* on GCC/Clang, standard C it is undefined. I need to convert all instances where I am doing pointer arithmetic on void* to char or create a #define for byte
 * TODO: When dealing with sizes, do not use ints, use size_t. Its better for cross platform compatibility
 * TODO: Header is not aligned to a 8 byte boundary
 * TODO: Don't use global error code, apparently this is bad practice and its better to return error codes from the functions themselves
 * TODO: Add freelist implementation to speed up memory allocation and deallocation
 */

struct BuddyAllocator globalBuddyAllocator = { 0x0 };
int errorCode = NO_ERR;


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
static struct Header *split(struct Header *chunk, size_t requestedSize)
{
    // Recursion break condition
    if ((requestedSize + HEADER_SIZE) >= (chunk->size / 2))
    {
        return chunk;
    }

    void *memory = chunk;

    // Split the chunk in half, which can be achieved simply by subtracting 1 from the order
    size_t newOrder = (chunk->order - 1);
    size_t newSize = 1 << newOrder;

    // Create new buddy node
    struct Header *buddy = (struct Header *)((uintptr_t)memory ^ newSize);
    buddy->size = newSize - HEADER_SIZE;
    buddy->free = TRUE;
    buddy->order = newOrder;

    // Update existing chunk to new size and attach its buddy
    chunk->size = newSize - HEADER_SIZE;
    chunk->order = newOrder;

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
static byte *search(size_t requestedSize)
{
    // Linearly search linked list for now.
    // free memory lookups fast: O(lg(n)) time
    struct Header* node = (struct Header*)globalBuddyAllocator.start;
    struct Header *bestFit = NULL;
    while (node < globalBuddyAllocator.end)
    {
        // TODO: When a freelist is implemented, bestFit can be set to the root free node and the NULL check will not be required
        if (node->free && node->size > requestedSize)
        {
            // NOTE: This NULL check will go away once a freelist is implemented
            if (bestFit == NULL || node->size < bestFit->size)
            {
                bestFit = node;
            }
        }

        size_t next = (0b1 << node->order);
        node += next;
    }

    if (requestedSize < (bestFit->size / 2))
        bestFit = split(bestFit, requestedSize);

    return bestFit;
}

/**
 * DRAFT: Attemps to coalesce a memory region with its buddy to create one unified block of memory. 
 * This is only called/attempted with memory is freed. Memory must be an address that is managed by the global allocator. 
 * 
 * @param memory The memory region to coalesce with its neighbors
 */
static void coalesce(struct Header *memory)
{
    size_t memorySize = power(2, memory->order);

    // Get Buddy
    struct Header *buddy = (struct Header *)((uintptr_t)memory ^ memorySize);

    if (memory->order == globalBuddyAllocator.order || !buddy->free) return;

    struct Header *lower = buddy < memory ? buddy : memory;
    lower->order += 1;
    lower->size = power(2, lower->order) - HEADER_SIZE;

    coalesce(lower);
}

/**
 * TODO: maxOrder should not exceed the maximum amount of memory that can be allocated on a device
 * TODO: Return error code that maps to proper error that occurred, don't set global error code
 */
int buddyInitGlobal(size_t maxOrder)
{

    // Check if global buddy allocator has already been initialized
    if (globalBuddyAllocator.start != 0x0 || globalBuddyAllocator.initialized == TRUE)
    {
        return PREV_INIT;
    }

    // Ensure maxOrder is valid before proceeding to mmap call
    if (maxOrder <= 0 || maxOrder > MAX_ORDER)
    {
        return INVAL_ARG;
    }

    // We must add HEADER_SIZE here or the user may end up with less memory than they were expecting
    // IF we are adding more to something that is already a power two, the only option is to round up to the next power of 2
    int allocatorSize = (0b1 << maxOrder);

    // size cannot and should not be 0 here. If size is 0, there is a bug
#if DEBUG
    assert(size != 0);
#endif 

    void *memory = mmap(NULL, allocatorSize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    if (memory == MAP_FAILED) // The mmap call failed for some reason, return an OS error to the user. It would be good to add some more detailed errors into a buffer somewhere the user could read from
    {
        return OS_ERR;
    }

    // Initialize the global allocator with the starting address of the newly mapped memory region
    globalBuddyAllocator.start = memory;
    globalBuddyAllocator.order = maxOrder;
    globalBuddyAllocator.end = globalBuddyAllocator.start + allocatorSize;

    // Initialization was successful if we reach this point
    return SUCCESS;
}

void *buddyAlloc(size_t requestedSize)
{
    // TODO: We could update to return the minimum amount of space
    if (requestedSize > (0b1 << globalBuddyAllocator.order) || requestedSize <= 0)
    {
        return NULL;
    }
    
    byte *userMemory = search(requestedSize);
    if (userMemory != NULL)
    {
        struct Header* header = (struct Header*)userMemory;
        header->free = FALSE;
        return userMemory + HEADER_SIZE;
    }

    return NULL;
}

/**
 * TODO: Return error code that maps to proper error that occurred, don't set global error code
 */
void buddyFree(void* ptr) 
{
    struct Header* memory = ptr;
    memory -= 1;

    // Validate the gap between the start of the managed region and the address being freed
    // is a power of 2. 
    int result = (uintptr_t)memory ^ (uintptr_t)globalBuddyAllocator.start;
    if ((result & (result - 1)) != 0)
    {
        return;
    }

    memory->free = TRUE;

    // coalesce? 
    coalesce(memory);
}