#ifndef MATH_UTILS_H_HEADER_GUARD
#define MATH_UTILS_H_HEADER_GUARD

#include "type.h"
#include <stdlib.h>

inline int32 random_in_range(int32 start, int32 end)
{
	return rand() % (end-start+1) + start;
}

#endif //MATH_UTILS_H_HEADER_GUARD