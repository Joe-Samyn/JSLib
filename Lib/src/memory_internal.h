#ifndef MEMORYINTERNAL_H
#define MEMORYINTERNAL_H

#include <stdlib.h>
#include <uuid/uuid.h>
#include "type.h"


/**
 * Define alignment size for system. This ignores SIMD for now. 
 * On most 64 bit systems, 8 byte alignment is used
 * On most 32 bit systems, 4 byte alignment is used
 */
#if defined(__aarch64__)
/**
The minimum order for the buddy allocator to support ARM 64 alignment requirements. This results in an alignment
of 2^3 = 8 bytes.
*/ 
#define MIN_ORDER 3
#define MAX_ORDER 40 // Maximum order for ARM 64 systems, can be adjusted based on system capabilities
#elif defined(__x86_64__)
/**
The minimum order for the buddy allocator to support x86-64 alignment requirements. This results in an alignment
of 2^3 = 8 bytes.
*/
#define MIN_ORDER 3
#else
/**
The minimum order for the buddy allocator to support 32-bit system alignment requirements. This results in an alignment
of 2^2 = 4 bytes.
*/
#define MIN_ORDER 2
#endif

// Size of the metadata block
// Header needs to be aligned to multiple of 2
#define HEADER_SIZE sizeof(struct Header)


/**
 * Aligns a number x to the next multiple of y. Used to round up and get the 
 * largest memory size that is a multiple of y needed to store the data.
 */
#define align(x, y) ((((x) + (y - 1)) & ~((y) - 1)))



#endif