#include "matrix4x4.h"

namespace loco
{
	const Matrix4x4 Matrix4x4::identity{
		{ {
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		} }
	};
}
