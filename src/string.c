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


char* str_cpy(char* restrict dest, char* restrict src)  {
	
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

char* str_ncpy(char* restrict dest, char* restrict src, int n) {

	/*
		TODO:
			- n should never be < 0, so it should be unsigned 
			- dest and src could be null pointers
			- src can be const 
			- restrict may not be best b/c someone could use this method in scenario
			where pointers alias 
			- How do we ensure we stay in bounds of pointers and do not buffer overlfow
			- Research Defensive programming in C and buffer overflow prevention strategies
			- Research how strncpy works, it fills remaining space of dest with \0 if src
			shorter than n
			- How should we return dest? Do we need to rewind or use copy of pointer? 
	*/
	
	// Copy characters until either n is hit or '\0' is hit	
	int num_chars = 0;
	while (*src != '\0' && num_chars < n) {
		*dest = *src;

		dest++;
		src++;
		num_chars++;
	}

	// Reset pointers
	dest -= num_chars;
	src -= num_chars;

	return dest;
}
