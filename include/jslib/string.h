#ifndef JSSTR_H
#define JSSTR_H


/*
 * TODO: String functionality needed
 * get length of string
 * append character to string
 * prepend character to string 
 * copy string 
 * string compare 
 */

#define STR_END '\0'

typedef enum {
    // String operation completed successfully 
    STR_OK,
    
    // String operations resulted in a buffer overflow for one of the strings involved
    STR_OVERFLOW,

    // One of the string pointers involved in the string operation was null
    STR_NULL_PTR,
    
    // One of the parameters involved in the string operation is invalid
    STR_INVALID_OPERAND,
    
    // An unknown error occurred when trying to perform the string operation
    STR_UKNOWN_ERR,
    
    // The string that resulted from the operation is missing a null terminating character
    STR_MISSING_TERM
} StrResult;

/**
 * @brief Computes the length of a string 
 * @param str Pointer to the string
 * @param maxLength Maximum number of characters to count. This is used as a protection
 * against nonnull terminating strings
 * @return Number of characters in the string (excluding null terminator)
 * @note The function assumes `str` is a valid, null terminated string.
 */
int strLen(const char* str, int maxLength);

/**
 * @brief Creates copy of a string
 * @param dest The destination to copy the string into 
 * @param destSize The total length of the destination string
 * @param src The string to copy
 * @return A pointer to the copied string 
 */
StrResult strCpy(char* dest, unsigned int destSize, const char* src);

/**
 * @brief Copies at most a count, n, of a null terminated string
 * @param dest A pointer to the destination buffer to copy to
 * @param destSize The size of the dest in number of bytes, it must be at least larger than max_chars + 1 
 * @param src A pointer to the null terminated string to copy from 
 * @param n The maximum number of characters to copy from src
 * @return A StrResult indicating success or failure (e.g., STR_OK, STR_NULL_PTR)
 * @note The string will be copied up till the null terminating byte or max character count `n`
 * @note If the n character limit is reached before the end of src, the dest string will NOT have 
 * a null terminating character. 
 */
StrResult strNCpy(char* dest, unsigned int destSize, const char* src, unsigned int n);

/**
 * @brief Compares two null terminated strings lexagraphically 
 * @param lhs Pointer to first null terminated string to compare
 * @param rhs Pointer to second null terminated string to compare
 * @returns 0 if strings are equal, negative value if lhs is greater than rhs, and positive value if rhs is greater than lhs
 */
int strCmp(char* lhs, char* rhs);

/**
 * @brief Lexagraphically compares the first n characters of two null terminated strings
 * @param lhs Pointer to first null terminated string to compare
 * @param rhs Pointer to second null terminated string to compare
 * @param n The max number of characters to compare 
 * @returns 0 if strings are equal, negative value if lhs is greater than rhs, and positive value if rhs is greater than lhs
 */
int strNCmp(char* lhs, char* rhs, int n);
#endif


