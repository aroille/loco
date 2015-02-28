#ifndef MATRIX4x4_H_HEADER_GUARD
#define MATRIX4x4_H_HEADER_GUARD

#include "type.h"
#include "bx\macros.h"
#include "bx\float4x4_t.h"
#include <string>


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

		// methods
		std::string to_string() const;
	};

	//================================================================================
	inline Matrix4x4 Matrix4x4::operator*(Matrix4x4 const& in) const
	{
		Matrix4x4 result;
		bx::float4x4_mul(&result.f4x4, &in.f4x4, &f4x4);
		return result;
	}

	inline std::string Matrix4x4::to_string() const
	{
		std::string str;

		str += "[";
		for (uint32 i = 0; i < 4; i++)
		{
			for (uint32 j = 0; j < 4; j++)
			{
				str += std::to_string(val[j + i*j]);
				str += ", ";
			}
		}

		str += "]";
		return str;
	}

} // math
} // loco


#endif // MATRIX4x4_H_HEADER_GUARD
