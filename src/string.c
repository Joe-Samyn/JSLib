#include "jslib/string.h"

static void fillWithTerm(char* str, unsigned int dest_size, unsigned int index);

// TODO: 
// - Convert all naming conventions to camelCase 

int strLen(const char* str) {

    // TODO: 
    //  - What if str is null?
    
    int len = 0;
    char c = *str;
    while (c != '\0') {	
        len++;
        str++;
        c = *str;
    }

    return len;
}

StrResult strCpy(char* dest, unsigned int destSize, char* src)  {

    // TODO: Edge cases to solve
    //  - Dest could not be large enough to store all values of src

    // TODO: What if destSize == 0?
    
    if (!dest || !src)
    {
        return STR_NULL_PTR;
    }
    
    char* tempDest = dest;
    char* tempSrc = src;
    unsigned int index = 0;
    while(index < destSize && *tempSrc != '\0')
    {
        *tempDest = *tempSrc;
        tempDest++;
        tempSrc++;
        index++;
    }
    
    if (index < destSize)
    {
        fillWithTerm(tempDest, destSize, index);
        return STR_OK;
    }
    
    if(*(tempDest - 1) != '\0')
    {
        return STR_MISSING_TERM;
    }
    

    return STR_OK;
}


StrResult str_ncpy(char* dest, unsigned int destSize, char* src, unsigned int maxChars) {
    
    // Need to ensure there is room for a null terminating character
    if (destSize < maxChars + 1 || maxChars <= 0) {
        return STR_INVALID_OPERAND;
    }

    if (!dest || !src) {
        return STR_NULL_PTR;
    }


    unsigned int index = 0;
    char* tempDest = dest;
    char* tempSrc = src;
    while (*tempSrc != '\0' && index < maxChars) {

        *tempDest = *tempSrc;

        tempDest++;
        tempSrc++;
        index++;
    }

    // If the last string character is not a null terminator, raise a warning
    // This may be OK, as the caller could be writing into a preexisting buffer
    // that is null terminated. Return STR_MISSING_TERM so the caller can explicitly
    // decide what to do in this scenario
    if (index < maxChars || index < destSize)
    {
        fillWithTerm(tempDest, destSize, index);        
    }
    else if (*(tempDest-1) != '\0') {
        return STR_MISSING_TERM;
    }

    return STR_OK;
}

/**
 * @brief Fills the string with null terminating characters from index
 * to max_char.
 * @param str A pointer to the string to fill with null terminating characters
 * @param str_size The length of the string pointed to by str
 * @param index The index of str to start inserting null characters
 */
static void fillWithTerm(char* str, unsigned int dest_size, unsigned int index)
{
    char* temp = str;
    for(int i = index; i < dest_size; i++)
    {
        *temp = '\0';
        temp++;
    }
}
