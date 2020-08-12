#pragma once

#include <type_traits>
#include <memory>
#include <assert.h>

template<typename T, size_t C>
class Vector 
{
public:
	static_assert(std::is_arithmetic<T>::value, "Type must be number");

	Vector() 
	{
		data = new T[C];
	}

	~Vector()
	{
		delete[] data;
	}

	T& operator[](size_t index) const
	{
		assert(index < C, "Index out of bounds");
		return data[index];
	}

	template<typename Ty, size_t Cy>
	Vector<decltype(T(0) + Ty(0)), C> operator+(const Vector<Ty, Cy>& other) const
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
	Vector<decltype(T(0) - Ty(0)), C> operator-(const Vector<Ty, Cy>& other) const
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
	Vector<decltype(T(1) * Ty(1)), C> operator*(const Vector<Ty, Cy>& other) const
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
	Vector<decltype(T(1) / Ty(1)), C> operator/(const Vector<Ty, Cy>& other) const
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
	void operator+=(const Vector<Ty, Cy>& other) const
	{
		static_assert(C == Cy, "Vectors must be same size");

		for (size_t i = 0; i < C; i++)
		{
			(*this)[i] += other[i];
		}
	}

	template<typename Ty, size_t Cy>
	void operator-=(const Vector<Ty, Cy>& other) const
	{
		static_assert(C == Cy, "Vectors must be same size");

		for (size_t i = 0; i < C; i++)
		{
			(*this)[i] -= other[i];
		}
	}

	template<typename Ty, size_t Cy>
	void operator*=(const Vector<Ty, Cy>& other) const
	{
		static_assert(C == Cy, "Vectors must be same size");

		for (size_t i = 0; i < C; i++)
		{
			(*this)[i] *= other[i];
		}
	}

	template<typename Ty, size_t Cy>
	void operator/=(const Vector<Ty, Cy>& other) const
	{
		static_assert(C == Cy, "Vectors must be same size");

		for (size_t i = 0; i < C; i++)
		{
			(*this)[i] /= other[i];
		}
	}

	//implement copies and moves

private:
	T* data;
};