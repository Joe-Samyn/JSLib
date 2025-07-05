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
 * 
 */

MemoryPool globalPool = { };

/** ************ Internal Functions ************* */


/** ************ External Functions ************* */

