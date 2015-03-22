#ifndef MATRIX4x4_H_HEADER_GUARD
#define MATRIX4x4_H_HEADER_GUARD

#include "math_utils.h"
#include "vector4.h"

namespace loco
{
	namespace math
	{	
		// Matrix4x4
		// Column major storage
		struct Matrix4x4
		{
			// attributes
			float m[4][4];			// m[column][row]

			// static constants
			static const Matrix4x4 zero;
			static const Matrix4x4 identity;

			// Methods
			Matrix4x4() = default;
			Matrix4x4(Matrix4x4 const& in) = default;
			Matrix4x4(Vector4 const& column0, Vector4 const& column1, Vector4 const& column2, Vector4 const& column3);
			Matrix4x4(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33);

			Matrix4x4 operator+(Matrix4x4 const& in) const;
			Matrix4x4 operator-(Matrix4x4 const& in) const;
			Matrix4x4 operator*(Matrix4x4 const& in) const;
			Vector4 operator*(Vector4 const& in) const;

			Matrix4x4& operator*=(Matrix4x4 const& in);

			bool operator==(Matrix4x4 const& in) const;
			bool operator!=(Matrix4x4 const& in) const;

			Matrix4x4 transpose() const;
			float determinant() const;
			Matrix4x4 inverse() const;

			static bool near_equal(Matrix4x4 const& a, Matrix4x4 const& b, float delta = DELTA);
			static Matrix4x4 proj_persp(float vertical_fov, float aspect_ratio, float near, float far);
			static Matrix4x4 proj_ortho(float left, float right, float bottom, float top, float near, float far);

			static Matrix4x4 look_at(const Vector3& eye, const Vector3& at, const Vector3& up);

		};

		//================================================================================
		inline Matrix4x4::Matrix4x4(Vector4 const& column0, Vector4 const& column1, Vector4 const& column2, Vector4 const& column3)
		{
			m[0][0] = column0.x; m[1][0] = column1.x; m[2][0] = column2.x; m[3][0] = column3.x;
			m[0][1] = column0.y; m[1][1] = column1.y; m[2][1] = column2.y; m[3][1] = column3.y;
			m[0][2] = column0.z; m[1][2] = column1.z; m[2][2] = column2.z; m[3][2] = column3.z;
			m[0][3] = column0.w; m[1][3] = column1.w; m[2][3] = column2.w; m[3][3] = column3.w;
		}

		//================================================================================
		inline Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
		{
			m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
			m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
			m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
			m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
		}

		//================================================================================
		inline Matrix4x4 Matrix4x4::operator+(Matrix4x4 const& in) const
		{
			Matrix4x4 result;

			for (uint32 y = 0; y<4; y++)
			{
				for (uint32 x = 0; x<4; x++)
				{
					result.m[y][x] = m[y][x] + in.m[y][x];
				}
			}

			return result;
		}

		//================================================================================
		inline Matrix4x4 Matrix4x4::operator-(Matrix4x4 const& in) const
		{
			Matrix4x4 result;

			for (uint32 y = 0; y<4; y++)
			{
				for (uint32 x = 0; x<4; x++)
				{
					result.m[y][x] = m[y][x] - in.m[y][x];
				}
			}

			return result;
		}

		//================================================================================
		inline Matrix4x4 Matrix4x4::operator*(Matrix4x4 const& in) const
		{
			Matrix4x4 result;

			for (uint32 c = 0; c<4; c++)
			{
				for (uint32 r = 0; r<4; r++)
				{
					result.m[c][r] = 0.0f;
					for (uint32 i = 0; i<4; i++)
					{
						result.m[c][r] += m[i][r] * in.m[c][i];
					}
				}
			}

			return result;
		}

		//================================================================================
		inline Vector4 Matrix4x4::operator*(Vector4 const& in) const
		{
			Vector4 result(0.0f, 0.0f, 0.0f, 0.0f);

			result.x += m[0][0] * in.x;
			result.x += m[1][0] * in.y;
			result.x += m[2][0] * in.z;
			result.x += m[3][0] * in.w;

			result.y += m[0][1] * in.x;
			result.y += m[1][1] * in.y;
			result.y += m[2][1] * in.z;
			result.y += m[3][1] * in.w;

			result.z += m[0][2] * in.x;
			result.z += m[1][2] * in.y;
			result.z += m[2][2] * in.z;
			result.z += m[3][2] * in.w;

			result.w += m[0][3] * in.x;
			result.w += m[1][3] * in.y;
			result.w += m[2][3] * in.z;
			result.w += m[3][3] * in.w;

			return result;
		}

		//================================================================================
		inline Matrix4x4& Matrix4x4::operator*=(Matrix4x4 const& in)
		{
			*this = *this * in;
			return *this;
		}

		//================================================================================
		inline bool Matrix4x4::operator==(Matrix4x4 const& in) const
		{
			for (uint32 y = 0; y<4; y++)
			{
				for (uint32 x = 0; x<4; x++)
				{
					if (m[y][x] != in.m[y][x])
					{
						return false;
					}
				}
			}

			return true;
		}

		//================================================================================
		inline bool Matrix4x4::operator!=(Matrix4x4 const& in) const
		{
			return !(*this == in);
		}

		//================================================================================
		inline Matrix4x4 Matrix4x4::transpose() const
		{
			Matrix4x4 result;

			for (uint32 y = 0; y<4; y++)
			{
				for (uint32 x = 0; x<4; x++)
				{
					result.m[y][x] = m[x][y];
				}
			}
			return result;
		}

		//================================================================================
		inline float Matrix4x4::determinant() const
		{
			return	m[0][0] * (
				m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
				m[2][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) +
				m[3][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2])
				) -
				m[1][0] * (
				m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
				m[2][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
				m[3][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2])
				) +
				m[2][0] * (
				m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) -
				m[1][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
				m[3][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
				) -
				m[3][0] * (
				m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) -
				m[1][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2]) +
				m[2][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
				);
		}

		//================================================================================
		inline Matrix4x4 Matrix4x4::inverse() const
		{
			Matrix4x4 result;

			float fTmp[4][4];
			float fTmpDet[4];

			fTmp[0][0] = m[2][2] * m[3][3] - m[2][3] * m[3][2];
			fTmp[0][1] = m[1][2] * m[3][3] - m[1][3] * m[3][2];
			fTmp[0][2] = m[1][2] * m[2][3] - m[1][3] * m[2][2];

			fTmp[1][0] = m[2][2] * m[3][3] - m[2][3] * m[3][2];
			fTmp[1][1] = m[0][2] * m[3][3] - m[0][3] * m[3][2];
			fTmp[1][2] = m[0][2] * m[2][3] - m[0][3] * m[2][2];

			fTmp[2][0] = m[1][2] * m[3][3] - m[1][3] * m[3][2];
			fTmp[2][1] = m[0][2] * m[3][3] - m[0][3] * m[3][2];
			fTmp[2][2] = m[0][2] * m[1][3] - m[0][3] * m[1][2];

			fTmp[3][0] = m[1][2] * m[2][3] - m[1][3] * m[2][2];
			fTmp[3][1] = m[0][2] * m[2][3] - m[0][3] * m[2][2];
			fTmp[3][2] = m[0][2] * m[1][3] - m[0][3] * m[1][2];

			fTmpDet[0] = m[1][1] * fTmp[0][0] - m[2][1] * fTmp[0][1] + m[3][1] * fTmp[0][2];
			fTmpDet[1] = m[0][1] * fTmp[1][0] - m[2][1] * fTmp[1][1] + m[3][1] * fTmp[1][2];
			fTmpDet[2] = m[0][1] * fTmp[2][0] - m[1][1] * fTmp[2][1] + m[3][1] * fTmp[2][2];
			fTmpDet[3] = m[0][1] * fTmp[3][0] - m[1][1] * fTmp[3][1] + m[2][1] * fTmp[3][2];

			float fDet = m[0][0] * fTmpDet[0] - m[1][0] * fTmpDet[1] + m[2][0] * fTmpDet[2] - m[3][0] * fTmpDet[3];
			float fInvDet = 1.0f / fDet;

			result.m[0][0] = fInvDet * fTmpDet[0];
			result.m[0][1] = -fInvDet * fTmpDet[1];
			result.m[0][2] = fInvDet * fTmpDet[2];
			result.m[0][3] = -fInvDet * fTmpDet[3];
			result.m[1][0] = -fInvDet * (m[1][0] * fTmp[0][0] - m[2][0] * fTmp[0][1] + m[3][0] * fTmp[0][2]);
			result.m[1][1] = fInvDet * (m[0][0] * fTmp[1][0] - m[2][0] * fTmp[1][1] + m[3][0] * fTmp[1][2]);
			result.m[1][2] = -fInvDet * (m[0][0] * fTmp[2][0] - m[1][0] * fTmp[2][1] + m[3][0] * fTmp[2][2]);
			result.m[1][3] = fInvDet * (m[0][0] * fTmp[3][0] - m[1][0] * fTmp[3][1] + m[2][0] * fTmp[3][2]);
			result.m[2][0] = fInvDet * (
				m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
				m[2][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) +
				m[3][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1])
				);
			result.m[2][1] = -fInvDet * (
				m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
				m[2][0] * (m[0][1] * m[3][3] - m[0][3] * m[3][1]) +
				m[3][0] * (m[0][1] * m[2][3] - m[0][3] * m[2][1])
				);
			result.m[2][2] = fInvDet * (
				m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) -
				m[1][0] * (m[0][1] * m[3][3] - m[0][3] * m[3][1]) +
				m[3][0] * (m[0][1] * m[1][3] - m[0][3] * m[1][1])
				);
			result.m[2][3] = -fInvDet * (
				m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) -
				m[1][0] * (m[0][1] * m[2][3] - m[0][3] * m[2][1]) +
				m[2][0] * (m[0][1] * m[1][3] - m[0][3] * m[1][1])
				);
			result.m[3][0] = -fInvDet * (
				m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
				m[2][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) +
				m[3][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
				);
			result.m[3][1] = fInvDet * (
				m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
				m[2][0] * (m[0][1] * m[3][2] - m[0][2] * m[3][1]) +
				m[3][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1])
				);
			result.m[3][2] = -fInvDet * (
				m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) -
				m[1][0] * (m[0][1] * m[3][2] - m[0][2] * m[3][1]) +
				m[3][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1])
				);
			result.m[3][3] = fInvDet * (
				m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
				m[1][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1]) +
				m[2][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1])
				);

			return result;
		}

		//================================================================================
		inline bool Matrix4x4::near_equal(Matrix4x4 const& a, Matrix4x4 const& b, float delta)
		{
			for (uint32 y = 0; y<4; y++)
			{
				for (uint32 x = 0; x<4; x++)
				{
					if (!math::near_equal(a.m[y][x], b.m[y][x], delta))
					{
						return false;
					}
				}
			}
			return true;
		}
		
		//================================================================================
		inline Matrix4x4 Matrix4x4::proj_persp(float vertical_fov, float aspect_ratio, float near, float far)
		{
			Matrix4x4 result(Matrix4x4::zero);

			const float diff = far - near;
			const float aa = far / diff;
			const float bb = - near * aa;

			const float height = 1.0f / tanf(DEG_TO_RAD * vertical_fov * 0.5f);
			const float width = height * 1.0f / aspect_ratio;

			result.m[0][0] = width;
			result.m[1][1] = height;
			result.m[2][2] = aa;
			result.m[2][3] = 1.0f;
			result.m[3][2] = bb;

			return result;
		}

		//================================================================================
		inline Matrix4x4 Matrix4x4::proj_ortho(float left, float right, float bottom, float top, float near, float far)
		{
			Matrix4x4 result(Matrix4x4::zero);

			const float aa = 2.0f / (right - left);
			const float bb = 2.0f / (top - bottom);
			const float cc = 1.0f / (far - near);
			const float dd = (left + right) / (left - right);
			const float ee = (top + bottom) / (bottom - top);
			const float ff = near / (near - far);

			result.m[0][0] = aa;
			result.m[1][1] = bb;
			result.m[2][2] = cc;
			result.m[3][0] = dd;
			result.m[3][1] = ee;
			result.m[3][2] = ff;
			result.m[3][3] = 1.0f;

			return result;
		}

		//================================================================================
		inline Matrix4x4 Matrix4x4::look_at(const Vector3& eye, const Vector3& at, const Vector3& up_dir)
		{
			Matrix4x4 result(Matrix4x4::zero);

			Vector3 view = (at - eye).normalize();
			Vector3 right = Vector3::cross(up_dir, view).normalize();
			Vector3 up = Vector3::cross(view, right);

			result.m[0][0] = right.x;
			result.m[1][0] = right.y;
			result.m[2][0] = right.z;

			result.m[0][1] = up.x;
			result.m[1][1] = up.y;
			result.m[2][1] = up.z;

			result.m[0][2] = view.x;
			result.m[1][2] = view.y;
			result.m[2][2] = view.z;

			result.m[3][0] = -Vector3::dot(right, eye);
			result.m[3][1] = -Vector3::dot(up, eye);
			result.m[3][2] = -Vector3::dot(view, eye);
			result.m[3][3] = 1.0f;

			return result;
		}

	} // math
} // loco


#endif // MATRIX4x4_H_HEADER_GUARD
