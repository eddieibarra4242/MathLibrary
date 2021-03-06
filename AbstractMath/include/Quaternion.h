#pragma once

#include <cmath>
#include "Vector.h"
#include "Matrix.h"

namespace AbstractMath {

	template<typename T>
	class Quaternion : public Vector<T, 4>
	{
	public:
		constexpr Quaternion(T x = T(0), T y = T(0), T z = T(0), T w = T(1))
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		constexpr Quaternion(const Vector<T, 3>& axis, T angle)
		{
			T sin = std::sin(angle / 2.0);
			T cos = std::cos(angle / 2.0);

			this->x = axis[0] * sin;
			this->y = axis[1] * sin;
			this->z = axis[2] * sin;
			this->w = cos;
		}

		constexpr Quaternion(const Vector<T, 4>& other) : Vector<T, 4>()
		{
			this->copyFrom(other.data);
		}

		constexpr Quaternion<T> conjugate() const
		{
			return Quaternion<T>(-(this->x), -(this->y), -(this->z), this->w);
		}

		template<typename Ty>
		constexpr Quaternion<decltype(T(1)* Ty(1))> operator*(const Quaternion<Ty>& other) const
		{
			using return_type = decltype(T(1)* Ty(1));
			return_type w = this->w * other.w - this->x * other.x - this->y * other.y - this->z * other.z;
			return_type x = this->w * other.x + this->x * other.w + this->y * other.z - this->z * other.y;
			return_type y = this->w * other.y + this->y * other.w + this->z * other.x - this->x * other.z;
			return_type z = this->w * other.z + this->z * other.w + this->x * other.y - this->y * other.x;

			return Quaternion<return_type>(x, y, z, w);
		}

		template<typename Ty>
		constexpr Quaternion<decltype(T(1)* Ty(1))> operator*(const Vector<Ty, 3>& other) const
		{
			using return_type = decltype(T(1)* Ty(1));
			return_type w = -(this->x * other[0]) - this->y * other[1] - this->z * other[2];
			return_type x = this->w * other[0] + this->y * other[2] - this->z * other[1];
			return_type y = this->w * other[1] + this->z * other[0] - this->x * other[2];
			return_type z = this->w * other[2] + this->x * other[1] - this->y * other[0];

			return Quaternion<return_type>(x, y, z, w);
		}

		template<typename Ty>
		constexpr Quaternion<decltype(T(1) / Ty(1))> operator/(const Quaternion<Ty>& other) const
		{
			using return_type = decltype(T(1) / Ty(1));
			return_type w = this->w * other.w + this->x * other.x + this->y * other.y + this->z * other.z;
			return_type x = this->x * other.w - this->w * other.x - this->y * other.z + this->z * other.y;
			return_type y = this->y * other.w - this->w * other.y - this->z * other.x + this->x * other.z;
			return_type z = this->z * other.w - this->w * other.z - this->x * other.y + this->y * other.x;

			return Quaternion<return_type>(x, y, z, w);
		}

		template<typename Ty>
		constexpr void operator*=(const Quaternion<Ty>& other)
		{
			this->w = this->w * other.w - this->x * other.x - this->y * other.y - this->z * other.z;
			this->x = this->w * other.x + this->x * other.w + this->y * other.z - this->z * other.y;
			this->y = this->w * other.y + this->y * other.w + this->z * other.x - this->x * other.z;
			this->z = this->w * other.z + this->z * other.w + this->x * other.y - this->y * other.x;
		}

		template<typename Ty>
		constexpr void operator/=(const Quaternion<Ty>& other)
		{
			this->w = this->w * other.w + this->x * other.x + this->y * other.y + this->z * other.z;
			this->x = this->x * other.w - this->w * other.x - this->y * other.z + this->z * other.y;
			this->y = this->y * other.w - this->w * other.y - this->z * other.x + this->x * other.z;
			this->z = this->z * other.w - this->w * other.z - this->x * other.y + this->y * other.x;
		}

		template<typename Ty>
		constexpr Vector<decltype(T(1)* Ty(1)), 3> rotate(const Vector<Ty, 3>& other) const
		{
			Quaternion<decltype(T(1)* Ty(1))> pure = conjugate() * other * (*this);
			return { pure.x, pure.y, pure.z };
		}

		constexpr Vector<T, 3> eulerAngles() const
		{
			return Vector<T, 3>{ std::atan2(2 * (this->w * this->x + this->y * this->z), 1 - 2 * (this->x * this->x + this->y * this->y)),
				std::asin(2 * (this->w * this->y - this->x * this->z)),
				std::atan2(2 * (this->w * this->z + this->x * this->y), 1 - 2 * (this->y * this->y + this->z * this->z)) };
		}

		constexpr T getRoll() const { return std::atan2(2 * (this->w * this->x + this->y * this->z), 1 - 2 * (this->x * this->x + this->y * this->y)); }
		constexpr T getPitch() const { return std::asin(2 * (this->w * this->y - this->x * this->z)); }
		constexpr T getYaw() const { return std::atan2(2 * (this->w * this->z + this->x * this->y), 1 - 2 * (this->y * this->y + this->z * this->z)); }

		constexpr Vector<T, 3> getRight()	const { Vector<T, 3> res = { this->x * this->x - this->y * this->y - this->z * this->z + this->w * this->w,  2.0 * this->x * this->y + 2.0 * this->z * this->w, 2.0 * this->x * this->z - 2.0 * this->y * this->w }; return res.normalized(); }
		constexpr Vector<T, 3> getLeft()	const { Vector<T, 3> res = { -this->x * this->x + this->y * this->y + this->z * this->z - this->w * this->w, -2.0 * this->x * this->y - 2.0 * this->z * this->w, 2.0 * this->y * this->w - 2.0 * this->x * this->z }; return res.normalized(); }
		constexpr Vector<T, 3> getUp()		const { Vector<T, 3> res = { 2.0 * this->x * this->y - 2.0 * this->z * this->w, -this->x * this->x + this->y * this->y - this->z * this->z + this->w * this->w,  2.0 * this->y * this->z + 2.0 * this->x * this->w }; return res.normalized(); }
		constexpr Vector<T, 3> getDown()	const { Vector<T, 3> res = { 2.0 * this->z * this->w - 2.0 * this->x * this->y,  this->x * this->x - this->y * this->y + this->z * this->z - this->w * this->w, -2.0 * this->y * this->z - 2.0 * this->x * this->w }; return res.normalized(); }
		constexpr Vector<T, 3> getForward() const { Vector<T, 3> res = { 2.0 * this->x * this->z + 2.0 * this->y * this->w, 2.0 * this->y * this->z - 2.0 * this->x * this->w, -this->x * this->x - this->y * this->y + this->z * this->z + this->w * this->w }; return res.normalized(); }
		constexpr Vector<T, 3> getBack()	const { Vector<T, 3> res = { -2.0 * this->x * this->z - 2.0 * this->y * this->w, 2.0 * this->x * this->w - 2.0 * this->y * this->z,  this->x * this->x + this->y * this->y - this->z * this->z - this->w * this->w }; return res.normalized(); }

		constexpr Matrix<T, 4, 4> toRotationMatrix()
		{
			Matrix<T, 4, 4> result;

			result[0][0] = T(1) - T(2) * (this->y * this->y + this->z * this->z); result[1][0] = T(2) * (this->x * this->y - this->w * this->z);		result[2][0] = T(2) * (this->x * this->z + this->w * this->y);		   result[3][0] = T(0);
			result[0][1] = T(2) * (this->x * this->y + this->w * this->z);		  result[1][1] = T(1) - T(2) * (this->x * this->x + this->z * this->z); result[2][1] = T(2) * (this->y * this->z - this->w * this->x);		   result[3][1] = T(0);
			result[0][2] = T(2) * (this->x * this->z - this->w * this->y);		  result[1][2] = T(2) * (this->y * this->z + this->w * this->x);		result[2][2] = T(1) - T(2) * (this->x * this->x + this->y * this->y); result[3][2] = T(0);
			
			result[0][3] = T(0); result[1][3] = T(0); result[2][3] = T(0); result[3][3] = T(1);

			return result;
		}

		template<typename Ty>
		constexpr Quaternion<decltype(T(0) + Ty(0))> operator+(const Ty& other) = delete;
		template<typename Ty>
		constexpr Quaternion<decltype(T(0) - Ty(0))> operator-(const Ty& other) = delete;
		template<typename Ty>
		constexpr Quaternion<decltype(T(1)* Ty(1))> operator*(const Ty& other) = delete;
		template<typename Ty>
		constexpr Quaternion<decltype(T(1) / Ty(1))> operator/(const Ty& other) = delete;

		template<typename Ty>
		constexpr Quaternion<decltype(T(0) + Ty(0))> operator+=(const Ty& other) = delete;
		template<typename Ty>
		constexpr Quaternion<decltype(T(0) - Ty(0))> operator-=(const Ty& other) = delete;
		template<typename Ty>
		constexpr Quaternion<decltype(T(1)* Ty(1))> operator*=(const Ty& other) = delete;
		template<typename Ty>
		constexpr Quaternion<decltype(T(1) / Ty(1))> operator/=(const Ty& other) = delete;
	};

	typedef Quaternion<float> Quaternionf;
	typedef Quaternion<double> Quaterniond;

}