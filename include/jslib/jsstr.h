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


/**
 * @brief Computes the length of a string 
 * @param str Pointer to the string
 * @return Number of characters in the string (excluding null terminator)
 * @note The function assumes `str` is a valid, null terminated string.
 */
int str_len(const char* str);

#endif


