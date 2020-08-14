#pragma once

#include <type_traits>
#include <memory>
#include <assert.h>

template<typename T, size_t C>
class Vector 
{
	static_assert(std::is_arithmetic<T>::value, "Type must be number");

protected:
	T* data;

public:
	constexpr Vector() 
	{
		data = new T[C];
	}

	constexpr Vector(const std::initializer_list<T>& l)
	{
		data = new T[C];
		memcpy_s(data, C * sizeof(T), l.begin(), l.size() * sizeof(T));
	}

	~Vector()
	{
		delete[] data;
	}

	constexpr Vector(const Vector<T, C>& other)
	{
		data = new T[C];
		memcpy_s(data, C * sizeof(T), other.data, C * sizeof(T));
	}

	constexpr void operator=(const Vector<T, C>& other)
	{
		memcpy_s(data, C * sizeof(T), other.data, C * sizeof(T));
	}

	constexpr Vector(Vector<T, C>&& other) noexcept
	{
		data = other.data;
		other.data = nullptr;
	}

	constexpr void operator=(Vector<T, C>&& other) noexcept
	{
		delete[] data;
		data = other.data;
		other.data = nullptr;
	}

	template<typename Ty, size_t Cy>
	constexpr Vector(const Vector<Ty, Cy>& other)
	{
		static_assert(C == Cy, "Vectors must be same size");
		data = new T[C];

		for (size_t i = 0; i < C; i++)
		{
			data[i] = other[i];
		}
	}

	template<typename Ty, size_t Cy>
	constexpr void operator=(const Vector<Ty, Cy>& other)
	{
		static_assert(C == Cy, "Vectors must be same size");

		for (size_t i = 0; i < C; i++)
		{
			data[i] = other[i];
		}
	}

	constexpr T& operator[](size_t index) const
	{
		assert(index < C);
		return data[index];
	}

	template<typename Ty, size_t Cy>
	constexpr Vector<decltype(T(0) + Ty(0)), C> operator+(const Vector<Ty, Cy>& other) const
	{
		static_assert(C == Cy, "Vectors must be same size");

		Vector<decltype(T(0) + Ty(0)), C> result;

		for (size_t i = 0; i < C; i++)
		{
			result[i] = (*this)[i] + other[i];
		}

		return result;
	}

	template<typename Ty, size_t Cy>
	constexpr Vector<decltype(T(0) - Ty(0)), C> operator-(const Vector<Ty, Cy>& other) const
	{
		static_assert(C == Cy, "Vectors must be same size");

		Vector<decltype(T(0) - Ty(0)), C> result;

		for (size_t i = 0; i < C; i++)
		{
			result[i] = (*this)[i] - other[i];
		}

		return result;
	}

	template<typename Ty, size_t Cy>
	constexpr Vector<decltype(T(1) * Ty(1)), C> operator*(const Vector<Ty, Cy>& other) const
	{
		static_assert(C == Cy, "Vectors must be same size");

		Vector<decltype(T(1) * Ty(1)), C> result;

		for (size_t i = 0; i < C; i++)
		{
			result[i] = (*this)[i] * other[i];
		}

		return result;
	}

	template<typename Ty, size_t Cy>
	constexpr Vector<decltype(T(1) / Ty(1)), C> operator/(const Vector<Ty, Cy>& other) const
	{
		static_assert(C == Cy, "Vectors must be same size");

		Vector<decltype(T(1) / Ty(1)), C> result;

		for (size_t i = 0; i < C; i++)
		{
			result[i] = (*this)[i] / other[i];
		}

		return result;
	}

	template<typename Ty, size_t Cy>
	constexpr void operator+=(const Vector<Ty, Cy>& other) const
	{
		static_assert(C == Cy, "Vectors must be same size");

		for (size_t i = 0; i < C; i++)
		{
			(*this)[i] += other[i];
		}
	}

	template<typename Ty, size_t Cy>
	constexpr void operator-=(const Vector<Ty, Cy>& other) const
	{
		static_assert(C == Cy, "Vectors must be same size");

		for (size_t i = 0; i < C; i++)
		{
			(*this)[i] -= other[i];
		}
	}

	template<typename Ty, size_t Cy>
	constexpr void operator*=(const Vector<Ty, Cy>& other) const
	{
		static_assert(C == Cy, "Vectors must be same size");

		for (size_t i = 0; i < C; i++)
		{
			(*this)[i] *= other[i];
		}
	}

	template<typename Ty, size_t Cy>
	constexpr void operator/=(const Vector<Ty, Cy>& other) const
	{
		static_assert(C == Cy, "Vectors must be same size");

		for (size_t i = 0; i < C; i++)
		{
			(*this)[i] /= other[i];
		}
	}
};