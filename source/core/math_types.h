#ifndef MATH_TYPE_H_HEADER_GUARD
#define MATH_TYPE_H_HEADER_GUARD

#include "type.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4x4.h"


namespace loco
{
namespace math
{
	struct Vector2i
	{
		int32 x, y;
	};

	/*
	struct Quaternion
	{
		float x, y, z, w;
	};
	*/

	struct AABB
	{
		Vector3 min, max;
	};

	struct OOBB
	{
		Matrix4x4 tm;
		AABB aabb;
	};

} // math
} // loco

#endif //MATH_TYPE_H_HEADER_GUARD