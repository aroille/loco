#ifndef VECTOR2_H_HEADER_GUARD
#define VECTOR2_H_HEADER_GUARD

#include "math_utils.h"

namespace loco
{
	namespace math
	{

		struct Vector2
		{
			// Attributes
			float x, y;

			// Static constants
			static const Vector2 zero;
			static const Vector2 unit_x;
			static const Vector2 unit_y;
			static const Vector2 one;

			// Methods
			Vector2() = default;
			Vector2(Vector2 const& in) = default;
			Vector2(float x_val, float y_val);
			Vector2(float value);

			Vector2 operator-() const;
			Vector2 operator+(Vector2 const& in) const;
			Vector2 operator-(Vector2 const& in) const;
			Vector2 operator*(Vector2 const& in) const;
			Vector2 operator/(Vector2 const& in) const;

			Vector2& operator+=(Vector2 const& in);
			Vector2& operator-=(Vector2 const& in);
			Vector2& operator*=(Vector2 const& in);
			Vector2& operator/=(Vector2 const& in);

			bool operator==(Vector2 const& in) const;
			bool operator!=(Vector2 const& in) const;
			bool operator>(Vector2 const& in) const;
			bool operator<(Vector2 const& in) const;
			bool operator>=(Vector2 const& in) const;
			bool operator<=(Vector2 const& in) const;

			float norm() const;
			float square_norm() const;
			Vector2 normalize() const;

			static bool near_equal(Vector2 const& a, Vector2 const& b, float delta = DELTA);
			static float distance(Vector2 const& a, Vector2 const& b);
			static float square_distance(Vector2 const& a, Vector2 const& b);
			static float dot(Vector2 const& a, Vector2 const& b);
			static Vector2 reflection(Vector2 const& in, Vector2 const& normal);
			static Vector2 lerp(Vector2 const& a, Vector2 const& b, float value);
			static Vector2 lerp(Vector2 const& a, Vector2 const& b, Vector2 const& value);
		};


		//================================================================================
		inline Vector2::Vector2(float x_val, float y_val) : x(x_val), y(y_val)
		{

		}

		//================================================================================
		inline Vector2::Vector2(float value) : x(value), y(value)
		{

		}

		//================================================================================
		inline Vector2 Vector2::operator-() const
		{
			return Vector2(-x, -y);
		}

		//================================================================================
		inline Vector2 Vector2::operator+(Vector2 const& in) const
		{
			return Vector2(x + in.x, y + in.y);
		}

		//================================================================================
		inline Vector2 Vector2::operator-(Vector2 const& in) const
		{
			return Vector2(x - in.x, y - in.y);
		}

		//================================================================================
		inline Vector2 Vector2::operator*(Vector2 const& in) const
		{
			return Vector2(x * in.x, y * in.y);
		}

		//================================================================================
		inline Vector2 Vector2::operator/(Vector2 const& in) const
		{
			return Vector2(x / in.x, y / in.y);
		}

		//================================================================================
		inline Vector2& Vector2::operator+=(Vector2 const& in)
		{
			x += in.x;
			y += in.y;
			return *this;
		}

		//================================================================================
		inline Vector2& Vector2::operator-=(Vector2 const& in)
		{
			x -= in.x;
			y -= in.y;
			return *this;
		}

		//================================================================================
		inline Vector2& Vector2::operator*=(Vector2 const& in)
		{
			x *= in.x;
			y *= in.y;
			return *this;
		}

		//================================================================================
		inline Vector2& Vector2::operator/=(Vector2 const& in)
		{
			x /= in.x;
			y /= in.y;
			return *this;
		}

		//================================================================================
		inline bool Vector2::operator==(Vector2 const& in) const
		{
			return (x == in.x) && (y == in.y);
		}

		//================================================================================
		inline bool Vector2::operator!=(Vector2 const& in) const
		{
			return (x != in.x) || (y != in.y);
		}

		//================================================================================
		inline bool Vector2::operator>(Vector2 const& in) const
		{
			return (x > in.x) && (y > in.y);
		}

		//================================================================================
		inline bool Vector2::operator<(Vector2 const& in) const
		{
			return (x < in.x) && (y < in.y);
		}

		//================================================================================
		inline bool Vector2::operator>=(Vector2 const& in) const
		{
			return (x >= in.x) && (y >= in.y);
		}

		//================================================================================
		inline bool Vector2::operator<=(Vector2 const& in) const
		{
			return (x <= in.x) && (y <= in.y);
		}

		//================================================================================
		inline float Vector2::norm() const
		{
			return sqrt(x*x + y*y);
		}

		//================================================================================
		inline float Vector2::square_norm() const
		{
			return x*x + y*y;
		}

		//================================================================================
		inline Vector2 Vector2::normalize() const
		{
			return Vector2(x, y) / norm();
		}

		//================================================================================
		inline bool Vector2::near_equal(Vector2 const& a, Vector2 const& b, float delta)
		{
			return math::near_equal(a.x, b.x, delta) && math::near_equal(a.y, b.y, delta);
		}

		//================================================================================
		inline Vector2 Vector2::reflection(Vector2 const& in, Vector2 const& normal)
		{
			return   in - normal * (2.0f * Vector2::dot(in, normal));
		}

		//================================================================================
		inline float Vector2::distance(Vector2 const& a, Vector2 const& b)
		{
			return (a - b).norm();
		}

		//================================================================================
		inline float Vector2::square_distance(Vector2 const& a, Vector2 const& b)
		{
			return (a - b).square_norm();
		}

		//================================================================================
		inline float Vector2::dot(Vector2 const& a, Vector2 const& b)
		{
			return a.x*b.x + a.y*b.y;
		}

		//================================================================================
		inline Vector2 Vector2::lerp(Vector2 const& a, Vector2 const& b, float value)
		{
			return Vector2(math::lerp(a.x, b.x, value), math::lerp(a.y, b.y, value));
		}

		//================================================================================
		inline Vector2 Vector2::lerp(Vector2 const& a, Vector2 const& b, Vector2 const& value)
		{
			return Vector2(math::lerp(a.x, b.x, value.x), math::lerp(a.y, b.y, value.y));
		}




	} // namespace math
} // namespace loco

#endif // VECTOR2_H_HEADER_GUARD