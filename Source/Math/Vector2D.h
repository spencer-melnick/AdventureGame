#pragma once

#include <type_traits>

namespace Engine
{
	namespace Math
	{
		template <typename T,
				  typename = std::enable_if<std::is_arithmetic<T>::value> >
		struct VectorBase
		{
			T X;
			T Y;

			VectorBase() :
				X(T(0)), Y(T(0))
			{};
			VectorBase(T x1, T y1) :
				X(x1), Y(y1)
			{};
			VectorBase(const VectorBase& vector) :
				X(vector.X), Y(vector.Y)
			{};

			VectorBase& operator+=(const VectorBase& vector)
			{
				x += vector.x;
				y += vector.y;

				return *this;
			}

			VectorBase& operator-=(const VectorBase& vector)
			{
				x -= vector.x;
				y -= vector.y;

				return *this;
			}

			VectorBase& operator*=(const VectorBase& vector)
			{
				x *= vector.x;
				y *= vector.y;

				return *this;
			}

			VectorBase& operator*=(const double& factor)
			{
				x *= factor;
				y *= factor;

				return *this;
			}

			VectorBase& operator/=(const VectorBase& vector)
			{
				x /= vector.x;
				y /= vector.y;

				return *this;
			}

			VectorBase& operator/=(const double& factor)
			{
				x /= factor;
				y /= factor;

				return *this;
			}


			VectorBase operator+(const VectorBase& vector)
			{
				VectorBase v = *this;
				return v += vector;
			}

			VectorBase operator-(const VectorBase& vector)
			{
				VectorBase v = *this;
				return v -= vector;
			}

			VectorBase operator*(const VectorBase& vector)
			{
				VectorBase v = *this;
				return v *= vector;
			}

			VectorBase operator*(const double& factor)
			{
				VectorBase v = *this;
				return v *= factor;
			}

			VectorBase operator/(const VectorBase& vector)
			{
				VectorBase v = *this;
				return v /= vector;
			}

			VectorBase operator/(const double& factor)
			{
				VectorBase v = *this;
				return v *= factor;
			}


			bool operator==(const VectorBase& vector)
			{
				return ((x == vector.x) && (y == vector.y));
			}

			bool operator!=(const VectorBase& vector)
			{
				return ((x != vector.x) || (y != vector.y));
			}

			double magnitude()
			{
				return std::sqrt(x * x + y * y);
			}

			VectorBase& normalize()
			{
				double m = magnitude();

				if (m != 0.0) {
					x /= m;
					y /= m;
				}

				return *this;
			}
		};

		using Vector2D = VectorBase<float>;
		using Vector2Di = VectorBase<int>;
	}
}