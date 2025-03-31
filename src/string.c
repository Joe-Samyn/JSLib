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

// TODO: Need to add size_t dest_size paramters so that dest does not buffer overflow
StrResult str_ncpy(char* dest, const char* src, int n) {
  /*
    TODO:
    - How do we ensure we stay in bounds of pointers and do not buffer overlfow
    - Research Defensive programming in C and buffer overflow prevention strategies
    - Research how strncpy works, it fills remaining space of dest with \0 if src
    shorter than n
    - How should we return dest? Do we need to rewind or use copy of pointer? 
  */
	

  if (n < 0) {
    return STR_INVALID_OPERAND;
  }

  if (!dest || !src) {
    return STR_NULL_PTR;
  }

  
  int num_chars = 0;
  char* temp = dest;
  while (*src != '\0' && num_chars < n) {

    *temp = *src;

    temp++;
    src++;
    num_chars++;
  }

  // if num_chars < n, fill dest with \0 until dest length == n
  while(num_chars < n) {
      *temp = '\0';

      temp++;
  }

  return STR_OK;
}
