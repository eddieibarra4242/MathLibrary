#pragma once

#include <type_traits>
#include <memory>
#include <assert.h>

namespace AbstractMath {

	template<typename T, size_t C, typename = void>
	struct BaseVectorData
	{
	public:
		union
		{
			T data[C] = { 0 };

			struct
			{
				union { T x, r, s; };
				union { T y, g, t; };
				union { T z, b, p; };
				union { T w, a, q; };
			};
		};

		constexpr BaseVectorData() = default;
	};

	template<typename T, size_t C>
	struct BaseVectorData<T, C, typename std::enable_if<C == 2>::type>
	{
	public:
		union
		{
			T data[C] = { 0 };

			struct
			{
				union { T x, r, s; };
				union { T y, g, t; };
			};
		};

		constexpr BaseVectorData() = default;
	};

	template<typename T, size_t C>
	struct BaseVectorData<T, C, typename std::enable_if<C == 3>::type>
	{
	public:
		union
		{
			T data[C] = { 0 };

			struct
			{
				union { T x, r, s; };
				union { T y, g, t; };
				union { T z, b, p; };
			};
		};

		constexpr BaseVectorData() = default;
	};

	template<typename T, size_t C>
	class Vector : public BaseVectorData<T, C>
	{
		static_assert(std::is_arithmetic<T>::value&& C > 1, "Type must be number and There must be more than one element!");

	protected:
		static const size_t SIZE = C * sizeof(T); //this shouldn't be used to determine class size! potential bug if used!

	public:
		using type = T;

		constexpr Vector() = default;

		constexpr Vector(const std::initializer_list<T>& l)
		{
			copyFrom(l.begin(), l.size() * sizeof(T));
		}

		constexpr Vector(const T* src, size_t srcSize)
		{
			assert(srcSize >= SIZE);
			copyFrom(src);
		}

		constexpr Vector(const Vector<T, C>& other)
		{
			copyFrom(other.data);
		}

		constexpr void operator=(const Vector<T, C>& other)
		{
			copyFrom(other.data);
		}

		template<typename Ty>
		constexpr Vector(const Vector<Ty, C>& other)
		{
			for (size_t i = 0; i < C; i++)
			{
				this->data[i] = other[i];
			}
		}

		template<typename Ty>
		constexpr void operator=(const Vector<Ty, C>& other)
		{
			for (size_t i = 0; i < C; i++)
			{
				this->data[i] = other[i];
			}
		}

		constexpr T& operator[](size_t index)
		{
			assert(index < C);
			return this->data[index];
		}

		constexpr const T& operator[](size_t index) const
		{
			assert(index < C);
			return this->data[index];
		}

		constexpr Vector<T, C> operator-() const
		{
			Vector<T, C> result;

			for (size_t i = 0; i < C; i++)
			{
				result[i] = -this->data[i];
			}

			return result;
		}

		template<typename Ty>
		constexpr Vector<decltype(T(0) + Ty(0)), C> operator+(const Vector<Ty, C>& other) const
		{
			Vector<decltype(T(0) + Ty(0)), C> result;

			for (size_t i = 0; i < C; i++)
			{
				result[i] = this->data[i] + other.data[i];
			}

			return result;
		}

		template<typename Ty>
		constexpr Vector<decltype(T(0) - Ty(0)), C> operator-(const Vector<Ty, C>& other) const
		{
			Vector<decltype(T(0) - Ty(0)), C> result;

			for (size_t i = 0; i < C; i++)
			{
				result[i] = this->data[i] - other.data[i];
			}

			return result;
		}

		template<typename Ty>
		constexpr Vector<decltype(T(1)* Ty(1)), C> operator*(const Vector<Ty, C>& other) const
		{
			Vector<decltype(T(1)* Ty(1)), C> result;

			for (size_t i = 0; i < C; i++)
			{
				result[i] = this->data[i] * other.data[i];
			}

			return result;
		}

		template<typename Ty>
		constexpr Vector<decltype(T(1) / Ty(1)), C> operator/(const Vector<Ty, C>& other) const
		{
			Vector<decltype(T(1) / Ty(1)), C> result;

			for (size_t i = 0; i < C; i++)
			{
				result[i] = this->data[i] / other.data[i];
			}

			return result;
		}

		template<typename Ty>
		constexpr Vector<decltype(T(0) + Ty(0)), C> operator+(const Ty& other) const
		{
			Vector<decltype(T(0) + Ty(0)), C> result;

			for (size_t i = 0; i < C; i++)
			{
				result[i] = this->data[i] + other;
			}

			return result;
		}

		template<typename Ty>
		constexpr Vector<decltype(T(0) - Ty(0)), C> operator-(const Ty& other) const
		{
			Vector<decltype(T(0) - Ty(0)), C> result;

			for (size_t i = 0; i < C; i++)
			{
				result[i] = this->data[i] - other;
			}

			return result;
		}

		template<typename Ty>
		constexpr Vector<decltype(T(1)* Ty(1)), C> operator*(const Ty& other) const
		{
			Vector<decltype(T(1)* Ty(1)), C> result;

			for (size_t i = 0; i < C; i++)
			{
				result[i] = this->data[i] * other;
			}

			return result;
		}

		template<typename Ty>
		constexpr Vector<decltype(T(1) / Ty(1)), C> operator/(const Ty& other) const
		{
			Vector<decltype(T(1) / Ty(1)), C> result;

			for (size_t i = 0; i < C; i++)
			{
				result[i] = this->data[i] / other;
			}

			return result;
		}

		template<typename Ty>
		constexpr void operator+=(const Vector<Ty, C>& other)
		{
			for (size_t i = 0; i < C; i++)
			{
				this->data[i] += other.data[i];
			}
		}

		template<typename Ty>
		constexpr void operator-=(const Vector<Ty, C>& other)
		{
			for (size_t i = 0; i < C; i++)
			{
				this->data[i] -= other.data[i];
			}
		}

		template<typename Ty>
		constexpr void operator*=(const Vector<Ty, C>& other)
		{
			for (size_t i = 0; i < C; i++)
			{
				this->data[i] *= other.data[i];
			}
		}

		template<typename Ty>
		constexpr void operator/=(const Vector<Ty, C>& other)
		{
			for (size_t i = 0; i < C; i++)
			{
				this->data[i] /= other.data[i];
			}
		}

		template<typename Ty, size_t Cy>
		constexpr bool operator<(const Vector<Ty, Cy>& other) const
		{
			return length() < other.length();
		}

		template<typename Ty, size_t Cy>
		constexpr bool operator>(const Vector<Ty, Cy>& other) const
		{
			return length() > other.length();
		}

		template<typename Ty>
		constexpr bool operator==(const Vector<Ty, C>& other) const
		{
			for (size_t i = 0; i < C; i++)
			{
				if (this->data[i] != other.data[i])
				{
					return false;
				}
			}

			return true;
		}

		constexpr T length() const
		{
			T sum = 0.0;

			for (size_t i = 0; i < C; i++)
			{
				sum += this->data[i] * this->data[i];
			}

			return T(sqrt(sum));
		}

		constexpr Vector<T, C> normalized() const
		{
			T len = length();
			return this->operator/(len);
		}

		template<typename Ty>
		constexpr decltype(T(1)* Ty(1)) dot(const Vector<Ty, C>& other)
		{
			using return_type = decltype(T(1)* Ty(1));
			return_type productSum = 0;

			for (size_t i = 0; i < C; i++)
			{
				productSum += this->data[i] * other.data[i];
			}

			return productSum;
		}

		template<typename Ty, typename Fy>
		constexpr Vector<decltype(T(0) + Ty(0)), C> lerp(const Vector<Ty, C>& other, Fy amount)
		{
			static_assert(std::is_floating_point<Fy>::value, "Amount Type must be a floating point number!");

			Vector<decltype(T(0) + Ty(0)), C> result;

			for (size_t i = 0; i < C; i++)
			{
				result[i] = (1 - amount) * this->data[i] + amount * other.data[i];
			}

			return result;
		}

	protected:
		constexpr void copyFrom(const T* src, size_t srcSize = SIZE)
		{
			memcpy_s(this->data, SIZE, src, srcSize);
		}
	};

	//template<typename T, size_t C>
	//constexpr Vector<T, C> i = { 1 };
	//template<typename T, size_t C>
	//constexpr Vector<T, C> j = { 0, 1 };
	//template<typename T, size_t C>
	//constexpr Vector<T, C> k = { 0, 0, 1 };
}