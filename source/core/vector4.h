#ifndef VECTOR4_H_HEADER_GUARD
#define VECTOR4_H_HEADER_GUARD

#include "math_utils.h"
#include "vector3.h"

namespace loco
{
	namespace math
	{

		struct Vector4
		{
			// Attributes
			float x, y, z, w;

			// Static constants
			static const Vector4 zero;
			static const Vector4 unit_x;
			static const Vector4 unit_y;
			static const Vector4 unit_z;
			static const Vector4 unit_w;
			static const Vector4 one;

			// Methods
			Vector4() = default;
			Vector4(Vector4 const& in) = default;
			Vector4(float x_val, float y_val, float z_val, float w_val);
			Vector4(Vector3 const& xyz_val, float w_val);
			Vector4(float value);

			Vector4 operator-() const;
			Vector4 operator+(Vector4 const& in) const;
			Vector4 operator-(Vector4 const& in) const;
			Vector4 operator*(Vector4 const& in) const;
			Vector4 operator/(Vector4 const& in) const;

			Vector4& operator+=(Vector4 const& in);
			Vector4& operator-=(Vector4 const& in);
			Vector4& operator*=(Vector4 const& in);
			Vector4& operator/=(Vector4 const& in);

			bool operator==(Vector4 const& in) const;
			bool operator!=(Vector4 const& in) const;
			bool operator>(Vector4 const& in) const;
			bool operator<(Vector4 const& in) const;
			bool operator>=(Vector4 const& in) const;
			bool operator<=(Vector4 const& in) const;

			float						norm() const;
			float						square_norm() const;
			Vector4					normalize() const;

			static bool			near_equal(Vector4 const& a, Vector4 const& b, float delta = DELTA);
			static float		distance(Vector4 const& a, Vector4 const& b);
			static float		square_distance(Vector4 const& a, Vector4 const& b);
			static float		dot(Vector4 const& a, Vector4 const& b);
			static Vector4	lerp(Vector4 const& a, Vector4 const& b, float value);
			static Vector4	lerp(Vector4 const& a, Vector4 const& b, Vector4 const& value);

			// The following methods ignore the coordinate W :
			float						norm3() const;
			float						square_norm3() const;

			static float		distance3(Vector4 const& a, Vector4 const& b);
			static Vector4	reflection3(Vector4 const& in, Vector4 const& normal);
			static float		square_distance3(Vector4 const& a, Vector4 const& b);
			static float		dot3(Vector4 const& a, Vector4 const& b);
			static Vector4	cross3(Vector4 const& a, Vector4 const& b);
		};




		//================================================================================
		inline Vector4::Vector4(float x_val, float y_val, float z_val, float w_val) : x(x_val), y(y_val), z(z_val), w(w_val)
		{

		}

		//================================================================================
		inline Vector4::Vector4(Vector3 const& xyz_vaL, float w_val) : x(xyz_vaL.x), y(xyz_vaL.y), z(xyz_vaL.z), w(w_val)
		{

		}

		//================================================================================
		inline Vector4::Vector4(float value) : x(value), y(value), z(value), w(value)
		{

		}

		//================================================================================
		inline Vector4 Vector4::operator-() const
		{
			return Vector4(-x, -y, -z, -w);
		}

		//================================================================================
		inline Vector4 Vector4::operator+(Vector4 const& in) const
		{
			return Vector4(x + in.x, y + in.y, z + in.z, w + in.w);
		}

		//================================================================================
		inline Vector4 Vector4::operator-(Vector4 const& in) const
		{
			return Vector4(x - in.x, y - in.y, z - in.z, w - in.w);
		}

		//================================================================================
		inline Vector4 Vector4::operator*(Vector4 const& in) const
		{
			return Vector4(x * in.x, y * in.y, z * in.z, w * in.w);
		}

		//================================================================================
		inline Vector4 Vector4::operator/(Vector4 const& in) const
		{
			return Vector4(x / in.x, y / in.y, z / in.z, w / in.w);
		}

		//================================================================================
		inline Vector4& Vector4::operator+=(Vector4 const& in)
		{
			x += in.x;
			y += in.y;
			z += in.z;
			w += in.w;
			return *this;
		}

		//================================================================================
		inline Vector4& Vector4::operator-=(Vector4 const& in)
		{
			x -= in.x;
			y -= in.y;
			z -= in.z;
			w -= in.w;
			return *this;
		}

		//================================================================================
		inline Vector4& Vector4::operator*=(Vector4 const& in)
		{
			x *= in.x;
			y *= in.y;
			z *= in.z;
			w *= in.w;
			return *this;
		}

		//================================================================================
		inline Vector4& Vector4::operator/=(Vector4 const& in)
		{
			x /= in.x;
			y /= in.y;
			z /= in.z;
			w /= in.w;
			return *this;
		}

		//================================================================================
		inline bool Vector4::operator==(Vector4 const& in) const
		{
			return (x == in.x) && (y == in.y) && (z == in.z) && (w == in.w);
		}

		//================================================================================
		inline bool Vector4::operator!=(Vector4 const& in) const
		{
			return (x != in.x) || (y != in.y) || (z != in.z) || (w != in.w);
		}

		//================================================================================
		inline bool Vector4::operator>(Vector4 const& in) const
		{
			return (x > in.x) && (y > in.y) && (z > in.z) && (w > in.w);
		}

		//================================================================================
		inline bool Vector4::operator<(Vector4 const& in) const
		{
			return (x < in.x) && (y < in.y) && (z < in.z) && (w < in.w);
		}

		//================================================================================
		inline bool Vector4::operator>=(Vector4 const& in) const
		{
			return (x >= in.x) && (y >= in.y) && (z >= in.z) && (w >= in.w);
		}

		//================================================================================
		inline bool Vector4::operator<=(Vector4 const& in) const
		{
			return (x <= in.x) && (y <= in.y) && (z <= in.z) && (w <= in.w);
		}

		//================================================================================
		inline float Vector4::norm() const
		{
			return sqrt(x*x + y*y + z*z + w*w);
		}

		//================================================================================
		inline float Vector4::norm3() const
		{
			return sqrt(x*x + y*y + z*z);
		}

		//================================================================================
		inline float Vector4::square_norm() const
		{
			return x*x + y*y + z*z + w*w;
		}

		//================================================================================
		inline float Vector4::square_norm3() const
		{
			return x*x + y*y + z*z;
		}

		//================================================================================
		inline Vector4 Vector4::normalize() const
		{
			return Vector4(x, y, z, w) / norm();
		}

		//================================================================================
		inline bool Vector4::near_equal(Vector4 const& a, Vector4 const& b, float delta)
		{
			return	math::near_equal(a.x, b.x, delta) &&
				math::near_equal(a.y, b.y, delta) &&
				math::near_equal(a.z, b.z, delta) &&
				math::near_equal(a.w, b.w, delta);
		}

		//================================================================================
		inline Vector4 Vector4::reflection3(Vector4 const& in, Vector4 const& normal)
		{
			return in - (normal * (2.f * Vector4::dot3(in, normal)));
		}

		//================================================================================
		inline float Vector4::distance(Vector4 const& a, Vector4 const& b)
		{
			return (a - b).norm();
		}

		//================================================================================
		inline float Vector4::distance3(Vector4 const& a, Vector4 const& b)
		{
			return (a - b).norm3();
		}

		//================================================================================
		inline float Vector4::square_distance(Vector4 const& a, Vector4 const& b)
		{
			return (a - b).square_norm();
		}

		//================================================================================
		inline float Vector4::square_distance3(Vector4 const& a, Vector4 const& b)
		{
			return (a - b).square_norm3();
		}

		//================================================================================
		inline float Vector4::dot(Vector4 const& a, Vector4 const& b)
		{
			return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
		}

		//================================================================================
		inline float Vector4::dot3(Vector4 const& a, Vector4 const& b)
		{
			return a.x*b.x + a.y*b.y + a.z*b.z;
		}

		//================================================================================
		inline Vector4 Vector4::cross3(Vector4 const& a, Vector4 const& b)
		{
			return Vector4(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x, 0.0f);
		}

		//================================================================================
		inline Vector4 Vector4::lerp(Vector4 const& a, Vector4 const& b, float value)
		{
			return Vector4(math::lerp(a.x, b.x, value), math::lerp(a.y, b.y, value), math::lerp(a.z, b.z, value), math::lerp(a.w, b.w, value));
		}

		//================================================================================
		inline Vector4 Vector4::lerp(Vector4 const& a, Vector4 const& b, Vector4 const& value)
		{
			return Vector4(math::lerp(a.x, b.x, value.x), math::lerp(a.y, b.y, value.y), math::lerp(a.z, b.z, value.z), math::lerp(a.w, b.w, value.w));
		}

	} // namespace math
} // namespace loco


#endif //VECTOR4_H_HEADER_GUARD