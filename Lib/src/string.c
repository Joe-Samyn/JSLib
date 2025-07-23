#include "jslib/string.h"

static void fillWithTerm(char* str, unsigned int dest_size, unsigned int index);

int strLen(const char* str, int maxLength) {
 
    if (!str || maxLength <= 0) {
        return STR_INVALID_OPERAND;
    }

    int len = 0;
    const char* temp = str;

    while (*temp != '\0' && len < maxLength) {	
        len++;
        temp++;
    }

    // If the len == maxLength then there is a possibility 
    // the string is missing a null terminating character
    // because the loop iterated until safety was hit. 
    // Return warning and let developer handle 
    if (len == maxLength) {
        return STR_MISSING_TERM;
    }

    return len;
}

StrResult strCpy(char* dest, unsigned int destSize, const char* src)  {

    if (!dest || !src)
    {
        return STR_NULL_PTR;
    }
    
    char* tempDest = dest;
    const char* tempSrc = src;
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

StrResult str_ncpy(char* dest, unsigned int destSize, const char* src, unsigned int maxChars) {
    
    if (destSize < maxChars) {
        return STR_INVALID_OPERAND;
    }

    if (!dest || !src) {
        return STR_NULL_PTR;
    }


    unsigned int index = 0;
    char* tempDest = dest;
    const char* tempSrc = src;
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
 * @param strSize The length of the string pointed to by str
 * @param index The index of str to start inserting null characters
 */
static void fillWithTerm(char* str, unsigned int strSize, unsigned int index)
{
    char* temp = str;
    for(int i = index; i < strSize; i++)
    {
        *temp = '\0';
        temp++;
    }
}
