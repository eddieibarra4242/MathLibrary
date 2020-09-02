#pragma once

#include "Vector.h"

template<typename T>
class Vector3 : public Vector<T, 3>
{
public:
	constexpr Vector3(T x = T(0), T y = T(0), T z = T(0))
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	constexpr Vector3(const Vector<T, 3>& other)
	{
		memcpy_s(this->data, 3 * sizeof(T), &other, 3 * sizeof(T));
	}

	template<typename Ty>
	constexpr Vector3<decltype(T(0) - Ty(0))> cross(const Vector3<Ty>& other)
	{
		using return_type = decltype(T(0) - Ty(0));
		return_type x = this->y * other.z - this->z * other.y;
		return_type y = this->z * other.x - this->x * other.z;
		return_type z = this->x * other.y - this->y * other.x;

		return Vector3<return_type>(x, y, z);
	}
};

typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;
typedef Vector3<int> Vector3i;