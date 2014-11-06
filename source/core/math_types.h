#ifndef MATH_TYPE_H_HEADER_GUARD
#define MATH_TYPE_H_HEADER_GUARD

#include "matrix4x4.h"

namespace loco
{
	struct Vector2
	{
		float x, y;
	};

	struct Vector3
	{
		float x, y, z;
	};

	struct Vector4
	{
		float x, y, z, w;
	};

	struct Quaternion
	{
		float x, y, z, w;
	};

	struct Matrix3x3
	{
		Vector3 x, y, z;
	};

	struct AABB
	{
		Vector3 min, max;
	};

	struct OOBB
	{
		Matrix4x4 tm;
		AABB aabb;
	};
}

#endif //MATH_TYPE_H_HEADER_GUARD