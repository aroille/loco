#ifndef VECTOR3_H_HEADER_GUARD
#define VECTOR3_H_HEADER_GUARD

#include "math_utils.h"

namespace loco
{
	namespace math
	{
		struct Vector3
		{
			// Attributes
			float x, y, z;

			// Static constants
			static const Vector3 zero;
			static const Vector3 unit_x;
			static const Vector3 unit_y;
			static const Vector3 unit_z;
			static const Vector3 one;

			// Methods
			Vector3() = default;
			Vector3(Vector3 const& in) = default;
			Vector3(float x_val, float y_val, float z_val);
			Vector3(float value);

			Vector3 operator-() const;
			Vector3 operator+(Vector3 const& in) const;
			Vector3 operator-(Vector3 const& in) const;
			Vector3 operator*(Vector3 const& in) const;
			Vector3 operator/(Vector3 const& in) const;

			Vector3& operator+=(Vector3 const& in);
			Vector3& operator-=(Vector3 const& in);
			Vector3& operator*=(Vector3 const& in);
			Vector3& operator/=(Vector3 const& in);

			bool operator==(Vector3 const& in) const;
			bool operator!=(Vector3 const& in) const;
			bool operator>(Vector3 const& in) const;
			bool operator<(Vector3 const& in) const;
			bool operator>=(Vector3 const& in) const;
			bool operator<=(Vector3 const& in) const;

			float						norm() const;
			float						square_norm() const;
			Vector3					normalize() const;

			static bool			near_equal(Vector3 const& a, Vector3 const& b, float delta = DELTA);
			static Vector3	reflection(Vector3 const& in, Vector3 const& normal);
			static float		distance(Vector3 const& a, Vector3 const& b);
			static float		square_distance(Vector3 const& a, Vector3 const& b);
			static float		dot(Vector3 const& a, Vector3 const& b);
			static Vector3	cross(Vector3 const& a, Vector3 const& b);
			static Vector3	lerp(Vector3 const& a, Vector3 const& b, float value);
			static Vector3	lerp(Vector3 const& a, Vector3 const& b, Vector3 const& value);
		};




		//================================================================================
		inline Vector3::Vector3(float x_value, float y_value, float z_val) : x(x_value), y(y_value), z(z_val)
		{

		}

		//================================================================================
		inline Vector3::Vector3(float value) : x(value), y(value), z(value)
		{

		}

		//================================================================================
		inline Vector3 Vector3::operator-() const
		{
			return Vector3(-x, -y, -z);
		}

		//================================================================================
		inline Vector3 Vector3::operator+(Vector3 const& in) const
		{
			return Vector3(x + in.x, y + in.y, z + in.z);
		}

		//================================================================================
		inline Vector3 Vector3::operator-(Vector3 const& in) const
		{
			return Vector3(x - in.x, y - in.y, z - in.z);
		}

		//================================================================================
		inline Vector3 Vector3::operator*(Vector3 const& in) const
		{
			return Vector3(x * in.x, y * in.y, z * in.z);
		}

		//================================================================================
		inline Vector3 Vector3::operator/(Vector3 const& in) const
		{
			return Vector3(x / in.x, y / in.y, z / in.z);
		}

		//================================================================================
		inline Vector3& Vector3::operator+=(Vector3 const& in)
		{
			x += in.x;
			y += in.y;
			z += in.z;
			return *this;
		}

		//================================================================================
		inline Vector3& Vector3::operator-=(Vector3 const& in)
		{
			x -= in.x;
			y -= in.y;
			z -= in.z;
			return *this;
		}

		//================================================================================
		inline Vector3& Vector3::operator*=(Vector3 const& in)
		{
			x *= in.x;
			y *= in.y;
			z *= in.z;
			return *this;
		}

		//================================================================================
		inline Vector3& Vector3::operator/=(Vector3 const& in)
		{
			x /= in.x;
			y /= in.y;
			z /= in.z;
			return *this;
		}

		//================================================================================
		inline bool Vector3::operator==(Vector3 const& in) const
		{
			return (x == in.x) && (y == in.y) && (z == in.z);
		}

		//================================================================================
		inline bool Vector3::operator!=(Vector3 const& in) const
		{
			return (x != in.x) || (y != in.y) || (z != in.z);
		}

		//================================================================================
		inline bool Vector3::operator>(Vector3 const& in) const
		{
			return (x > in.x) && (y > in.y) && (z > in.z);
		}

		//================================================================================
		inline bool Vector3::operator<(Vector3 const& in) const
		{
			return (x < in.x) && (y < in.y) && (z < in.z);
		}

		//================================================================================
		inline bool Vector3::operator>=(Vector3 const& in) const
		{
			return (x >= in.x) && (y >= in.y) && (z >= in.z);
		}

		//================================================================================
		inline bool Vector3::operator<=(Vector3 const& in) const
		{
			return (x <= in.x) && (y <= in.y) && (z <= in.z);
		}

		//================================================================================
		inline float Vector3::norm() const
		{
			return sqrt(x*x + y*y + z*z);
		}

		//================================================================================
		inline float Vector3::square_norm() const
		{
			return x*x + y*y + z*z;
		}

		//================================================================================
		inline Vector3 Vector3::normalize() const
		{
			return Vector3(x, y, z) / norm();
		}

		//================================================================================
		inline bool Vector3::near_equal(Vector3 const& a, Vector3 const& b, float delta)
		{
			return	math::near_equal(a.x, b.x, delta) &&
				math::near_equal(a.y, b.y, delta) &&
				math::near_equal(a.z, b.z, delta);
		}

		//================================================================================
		inline Vector3 Vector3::reflection(Vector3 const& in, Vector3 const& normal)
		{
			return   in - normal * (2.f * Vector3::dot(in, normal));
		}

		//================================================================================
		inline float Vector3::distance(Vector3 const& a, Vector3 const& b)
		{
			return (a - b).norm();
		}

		//================================================================================
		inline float Vector3::square_distance(Vector3 const& a, Vector3 const& b)
		{
			return (a - b).square_norm();
		}

		//================================================================================
		inline float Vector3::dot(Vector3 const& a, Vector3 const& b)
		{
			return a.x*b.x + a.y*b.y + a.z*b.z;
		}

		//================================================================================
		inline Vector3 Vector3::cross(Vector3 const& a, Vector3 const& b)
		{
			return Vector3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
		}

		//================================================================================
		inline Vector3 Vector3::lerp(Vector3 const& a, Vector3 const& b, float value)
		{
			return Vector3(math::lerp(a.x, b.x, value), math::lerp(a.y, b.y, value), math::lerp(a.z, b.z, value));
		}

		//================================================================================
		inline Vector3 Vector3::lerp(Vector3 const& a, Vector3 const& b, Vector3 const& value)
		{
			return Vector3(math::lerp(a.x, b.x, value.x), math::lerp(a.y, b.y, value.y), math::lerp(a.z, b.z, value.z));
		}

	} // namespace math
} // namespace loco

#endif // VECTOR3_H_HEADER_GUARD