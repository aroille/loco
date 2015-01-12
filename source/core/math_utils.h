#ifndef MATH_UTILS_H_HEADER_GUARD
#define MATH_UTILS_H_HEADER_GUARD

#include <stdlib.h>

inline int random_in_range(int start, int end)
{
	return rand() % (end-start+1) + start;
}

#endif //MATH_UTILS_H_HEADER_GUARD