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
