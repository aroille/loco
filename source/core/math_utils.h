#ifndef MATH_UTILS_H_HEADER_GUARD
#define MATH_UTILS_H_HEADER_GUARD

#include "type.h"
#include <math.h>
#include <stdlib.h>

namespace loco
{
	namespace math
	{

		// Useful mathematical const
		#define PI 					(3.1415926535897932f)
		#define INV_PI				(0.31830988618f)
		#define HALF_PI				(1.57079632679f)
		#define SMALL_NUMBER		(1.e-8f)
		#define KINDA_SMALL_NUMBER	(1.e-4f)
		#define EULERS_NUMBER       (2.71828182845904523536f)
		#define RAD_TO_DEG			(57.29577951308232f)
		#define DEG_TO_RAD			(0.017453292519943f)
		// Magic numbers for numerical precision
		#define DELTA				(0.00001f)

		inline float lerp(float a, float b, float value) { return a + value*(b - a); }
		inline float clamp(float in, float min, float max) { return (in < min ? min : (in > max ? max : in)); }
		inline bool  near_equal(float a, float b, float delta = DELTA) { return (abs(a - b) > delta ? false : true); }
		inline int32 random_in_range(int32 start, int32 end) { return rand() % (end - start + 1) + start; }

	} // namespace math
} // namesapce loco

#endif //MATH_UTILS_H_HEADER_GUARD