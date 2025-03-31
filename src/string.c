#include "jslib/string.h"


// TODO: We can create a safe_str_len function that takes a max length from the user
int str_len(const char* str) {

    // TODO: Is there a way to do this more performantly with SIMD or 
    // vectorized instructions?

    int len = 0;
    char c = *str;
    while (c != '\0') {	
        len++;
        str++;
        c = *str;
    }

    return len;
}

// TODO: Need to add a parameter, size_t dest_size, so that we can confirm we do not
// buffer overflow
// TODO(joe): Ensure dest and src are not null
// TODO(joe): Ensure dest ends with null terminating character
// TODO(joe): Return proper StrResult for each scenario
// TODO(joe): Copy dest pointer so dest pointer is not manipulated. Currently this
// will leave dest pointer pointing to end of string
char* str_cpy(char* dest, const char* src)  {

    // Loop over src using pointer arithmetic and copy characters into dest
    // TODO: Could also benefit from SIMD or vectorized instructions 
    char cursor = *src;
    while (cursor != STR_END) {
        *dest = cursor;

        src++;
        dest++;
        cursor = *src;
    }

    return dest;
}


StrResult str_ncpy(char* dest, unsigned int dest_size, const char* src, unsigned int max_chars) {
    
    // Need to ensure there is room for a null terminating character
    if (dest_size < max_chars + 1 || max_chars <= 0) {
        return STR_INVALID_OPERAND;
    }

    if (!dest || !src) {
        return STR_NULL_PTR;
    }


    int num_chars = 0;
    char* temp = dest;
    while (*src != '\0' && num_chars < max_chars) {

        *temp = *src;

        temp++;
        src++;
        num_chars++;
    }

    // if num_chars < max_chars, fill dest with \0 until dest length == max_chars
    // to ensure that dest ends with a null terminating character.
    while(num_chars < max_chars) {
        *temp = '\0';

        temp++;
        num_chars++;
    }        

    // If the last string character is not a null terminator, raise a warning
    // This may be OK, as the caller could be writing into a preexisting buffer
    // that is null terminated. Return STR_MISSING_TERM so the caller can explicitly
    // decide what to do in this scenario
    if (*(temp-1) != '\0') {
        return STR_MISSING_TERM;
    }

    return STR_OK;
}
