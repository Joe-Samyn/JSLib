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
 * TODO:
 *
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
static void *search(size_t requestedSize)
{

    // requestedSize must be greater than 0 to get a valid memory region
    if (requestedSize <= 0)
    {
        errorCode = INVAL_ARG;
        return NULL;
    }

    // Linearly search linked list for now.
    // TODO: This is not a good solution. We need a freelist implementation, preferable using a self balancing tree to make
    // free memory lookups fast: O(lg(n)) time
    void* node = globalBuddyAllocator.start;
    struct Header *bestFit = NULL;
    while (node < globalBuddyAllocator.end)
    {
        struct Header *temp = (struct Header*)node;

        // TODO: When a freelist is implemented, bestFit can be set to the root free node and the NULL check will not be required
        if (temp->free)
        {
            // NOTE: This NULL check will go away once a freelist is implemented
            if (bestFit == NULL)
            {
                bestFit = node;
            }
            else if (temp->size > requestedSize && temp->size < bestFit->size)
            {
                bestFit = temp;
            }
        }

        size_t next = power(2, temp->order);
        node += next;
    }

    if (bestFit == NULL)
    {
        errorCode = NO_MEM;
        return NULL;
    }

    if (requestedSize < (bestFit->size / 2))
        bestFit = split(bestFit, requestedSize);

    bestFit->free = FALSE;
    return bestFit;
}

/**
 * DRAFT: Attemps to coalesce a memory region with its buddy to create one unified block of memory. 
 * This is only called/attempted with memory is freed. Memory must be an address that is managed by the global allocator. 
 * 
 * @param memory The memory region to coalesce with its neighbors
 */
static int coalesce(struct Header *memory) 
{
    size_t memorySize = power(2, memory->order);

    // Get Buddy
    struct Header *buddy = (struct Header *)((uintptr_t)memory ^ memorySize);
    if (buddy->free) 
    {
        // Join 
    }

    return 0;
}


int buddyInitGlobal(size_t maxOrder)
{

    // Check if global buddy allocator has already been initialized
    if (globalBuddyAllocator.start != 0x0)
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

    void *memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    // If we get here, something else failed that the user has no control over. Let them know an OS level error occurred with the library
    if (memory == MAP_FAILED)
    {
        errorCode = OS_ERR;
        return ERROR;
    }

    // Convert the mapped memory region to a header struct to store metadata about the
    // newly obtained block of memory
    struct Header *header = (struct Header *)memory;
    header->size = size - HEADER_SIZE;
    header->free = TRUE;
    header->order = log2(size); // NOTE: We aligned maxOrder up to the system page size, so we need to calculate the new maxOrder

    // Initialize the global allocator with the starting address of the newly mapped memory region
    globalBuddyAllocator.start = header;
    globalBuddyAllocator.order = header->order;
    globalBuddyAllocator.end = globalBuddyAllocator.start + size;

    // Initialization was successful if we reach this point
    return SUCCESS;
}

void *buddyAlloc(size_t requestedSize)
{
    // TODO: We can probably use a ternary here and potentially a conditional data move which would result in better branch prediction
    struct Header *memory = search(requestedSize);
    return memory ? memory + 1 : memory;
}

void buddyFree(void* ptr) 
{
    struct Header* memory = ptr;
    memory -= 1;

    memory->free = TRUE;

    // coalesce? 
    coalesce(memory);
}