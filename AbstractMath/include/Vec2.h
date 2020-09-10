#pragma once

#include "Vector.h"

namespace AbstractMath {

	template<typename T>
	class Vector2 : public Vector<T, 2>
	{
	public:
		constexpr Vector2(T x = T(0), T y = T(0))
		{
			this->x = x;
			this->y = y;
		}

		constexpr Vector2(const Vector<T, 2>& other)
		{
			this->copyFrom(other.data);
		}

		template<typename Ty>
		constexpr decltype(T(0) - Ty(0)) cross(const Vector2<Ty>& other)
		{
			return this->x * other.y - this->y * other.x;
		}
	};

	typedef Vector2<float> Vector2f;
	typedef Vector2<double> Vector2d;
	typedef Vector2<int> Vector2i;

}