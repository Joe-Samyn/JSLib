#include "math.h"

int power(long base, long exponent) {
    int result = base;
    while(exponent != 0) 
    {
        result *= base;
        exponent--;
    }

    return result;
}