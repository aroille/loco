#ifndef MATRIX4x4_H_HEADER_GUARD
#define MATRIX4x4_H_HEADER_GUARD

#include "bx\macros.h"
#include "bx\float4x4_t.h"

namespace loco
{
namespace math
{
	BX_ALIGN_DECL_16(struct) Matrix4x4
	{
		// member
		union
		{
			float val[16];
			bx::float4x4_t f4x4;
		};

		// static constants
		static const Matrix4x4 identity;

		// operators
		Matrix4x4 operator*(Matrix4x4 const& in) const;
	};

	//================================================================================
	inline Matrix4x4 Matrix4x4::operator*(Matrix4x4 const& in) const
	{
		Matrix4x4 result;
		bx::float4x4_mul(&result.f4x4, &in.f4x4, &f4x4);
		return result;
	}
} // math
} // loco


#endif // MATRIX4x4_H_HEADER_GUARD