#ifndef MEMORYINTERNAL_H
#define MEMORYINTERNAL_H

#include <stdlib.h>
#include <uuid/uuid.h>
#include "type.h"


/** 
 * Number of bytes to align to when requesting memory from OS
 * TODO: This should be looked at further b/c not every machine will align to 8 bytes 
 * TODO: Calling a system call in a #define is not performant. Calls to system are expensive. This will place this
 * system call everywhere ALIGNMENT is used, which is bad. We need to rethink this. 
*/
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
#define HEADER_SIZE sizeof(Metadata)


/**
 * Aligns a number x to the next multiple of y. Used to round up and get the 
 * largest memory size that is a multiple of y needed to store the data.
 */
#define align(x, y) (((x) + (y - 1) & ~((y) - 1)))



#endif