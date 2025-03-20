#include "jslib/jsstr.h"


// TODO: We can create a safe_str_len function that takes a max length from the user
int str_len(const char* str) {
	
	// Easiest way is to iterator over the string 
	// one byte at a time until the '\0' char is 
	// found 
	
	int len = 0;
	char c = *str;
	while (c != '\0') {	
		len++;
		str++;
		c = *str;
	}

	return len;
}

