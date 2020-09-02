#pragma once

#include "Vector.h"

template<typename T>
class Vector4 : public Vector<T, 4>
{
public:
	constexpr Vector4(T x = T(0), T y = T(0), T z = T(0), T w = T(0))
	{
		this->data[0] = x;
		this->data[1] = y;
		this->data[2] = z;
		this->data[3] = w;
	}

	constexpr Vector4(const Vector<T, 4>& other)
	{
		memcpy_s(this->data, 4 * sizeof(T), other.data, 4 * sizeof(T));
	}
};

typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;
typedef Vector4<int> Vector4i;