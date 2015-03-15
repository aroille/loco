
#include "vector3.h"
#include "vector4.h"
#include "matrix4x4.h"

namespace loco{
namespace math{

	const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
	const Vector3 Vector3::unit_x(1.0f, 0.0f, 0.0f);
	const Vector3 Vector3::unit_y(0.0f, 1.0f, 0.0f);
	const Vector3 Vector3::unit_z(0.0f, 0.0f, 1.0f);
	const Vector3 Vector3::one(1.0f, 1.0f, 1.0f);

	const Vector4 Vector4::zero(0.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::unit_x(1.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::unit_y(0.0f, 1.0f, 0.0f, 0.0f);
	const Vector4 Vector4::unit_z(0.0f, 0.0f, 1.0f, 0.0f);
	const Vector4 Vector4::unit_w(0.0f, 0.0f, 0.0f, 1.0f);
	const Vector4 Vector4::one(1.0f, 1.0f, 1.0f, 1.0f);

	const Matrix4x4 Matrix4x4::identity = Matrix4x4{
			{ {
				1.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				0.f, 0.f, 0.f, 1.f
			} }
	};



} // math
}// loco
