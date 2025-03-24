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

/**
 * @brief Creates copy of a string
 * @param dest The destination to copy the string into 
 * @param src The string to copy
 * @return A pointer to the copied string 
 */
char* str_cpy(char* restrict dest, char* restrict src);

/**
 * @brief Copies at most a count, n, of a null terminated string
 * @param dest A pointer to the destination buffer to copy to
 * @param src A pointer to the null terminated string to copy from 
 * @param n The maximum number of characters to copy from src
 * @return A copy of a pointer to the destination string 
 * @note The string will be copied up till the null terminating byte or max character count `n`
 */
char* str_ncpy(char* restrict dest, char* restrict src, int n);

/**
 * @brief Concatenates a copy of the null terminated string pointed to by src to the end of the dest string
 * @param dest A pointer to the null terminated string to append to 
 * @param src A pointer to the null terminated string to copy from 
 * @param A pointer to dest that includes the appended copy of src
 */
char* str_cat(char* restrict dest, char* restrict src);

/**
 * @brief Compares two null terminated strings lexagraphically 
 * @param lhs Pointer to first null terminated string to compare
 * @param rhs Pointer to second null terminated string to compare
 * @returns 0 if strings are equal, negative value if lhs is greater than rhs, and positive value if rhs is greater than lhs
 */
int str_cmp(char* lhs, char* rhs);

/**
 * @brief Lexagraphically compares the first n characters of two null terminated strings
 * @param lhs Pointer to first null terminated string to compare
 * @param rhs Pointer to second null terminated string to compare
 * @param n The max number of characters to compare 
 * @returns 0 if strings are equal, negative value if lhs is greater than rhs, and positive value if rhs is greater than lhs
 */
int str_ncmp(char* lhs, char* rhs, int n);
#endif


