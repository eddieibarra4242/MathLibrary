 #pragma once

#include <type_traits>
#include <memory>
#include <assert.h>
#include <cmath>

#include "Vector.h"

namespace AbstractMath {

	template<typename T, size_t Rows_C, size_t Cols_C>
	class Matrix
	{
		static_assert(std::is_arithmetic<T>::value&& Rows_C > 0 && Cols_C > 0, "Type must be number; rows and columns must be non-zero");

	public:
		T data[Rows_C * Cols_C] = { 0 }; // treat as array of Columns vectors
		static const size_t SIZE = Rows_C * Cols_C * sizeof(T); //this shouldn't be used to determine class size! potential bug if used!

	public:

		constexpr Matrix() = default;

		constexpr Matrix(const std::initializer_list<T>& l)
		{
			transposeCopy(l.begin(), l.size());
		}

		constexpr Matrix(const Matrix<T, Rows_C, Cols_C>& other)
		{
			copyFrom(other.data);
		}

		template<typename Ty>
		constexpr Matrix(const Matrix<Ty, Rows_C, Cols_C>& other)
		{
			for (size_t i = 0; i < Rows_C * Cols_C; i++)
			{
				this->data[i] = other.data[i];
			}
		}

		constexpr void operator=(const Matrix<T, Rows_C, Cols_C>& other)
		{
			copyFrom(other.data);
		}

		constexpr Vector<T, Rows_C>& operator[](const size_t& index)
		{
			assert(index < Cols_C);
			return *(reinterpret_cast<Vector<T, Rows_C>*>(&data[index * Rows_C])); //weirdness with individual assignments, reinterpret is correct
		}

		constexpr Matrix<T, Cols_C, Rows_C> transposed() const
		{
			Matrix<T, Cols_C, Rows_C> transposedMat;
			transposedMat.transposeCopy(data);
			return transposedMat;
		}

		template<typename Ty, size_t R_Cols>
		constexpr Matrix<decltype(T(1)* Ty(1)), Rows_C, R_Cols> operator*(const Matrix<Ty, Cols_C, R_Cols>& right) const
		{
			using return_type = decltype(T(1)* Ty(1));
			Matrix<return_type, Rows_C, R_Cols> result;

			size_t leftIndex, rightIndex, destIndex = 0;
			size_t leftOffset = 0, rightOffset = 0;

			for (/*size_t destIndex = 0*/; destIndex < Rows_C * R_Cols; destIndex++)
			{
				return_type dot = 0;

				for (leftIndex = leftOffset, rightIndex = rightOffset; leftIndex < (Rows_C * Cols_C); rightIndex++, leftIndex += Rows_C)
				{
					dot += data[leftIndex] * right.data[rightIndex];
				}

				result.data[destIndex] = dot;

				leftOffset++;

				if (leftOffset == Rows_C)
				{
					leftOffset = 0;
					rightOffset += Cols_C;
				}
			}

			return result;
		}

		template<typename Ty>
		constexpr Vector<decltype(T(1)* Ty(1)), Rows_C> operator*(const Vector<Ty, Cols_C>& other) const
		{
			Vector<decltype(T(1)* Ty(1)), Rows_C> result;

			for (size_t matIn = 0; matIn < Rows_C * Cols_C; matIn++)
			{
				size_t rowIn = matIn % Rows_C;
				size_t colIn = matIn / Rows_C;

				result[rowIn] += data[matIn] * other[colIn];
			}

			return result;
		}

		//TODO: man up and implement inverse and determinate

	private:
		constexpr void copyFrom(const T* src, const size_t srcSize = SIZE)
		{
			memcpy_s(this->data, SIZE, src, srcSize);
		}

		constexpr void transposeCopy(const T* src, const size_t srcElements = (Rows_C * Cols_C))
		{
			static const size_t destElements = Rows_C * Cols_C;

			size_t srcIndex = 0;
			size_t offset = 0;

			for (size_t destIndex = 0; destIndex < destElements; destIndex++)
			{
				if (destIndex % Rows_C == 0)
				{
					srcIndex = offset;
					offset++;
				}

				data[destIndex] = srcIndex >= srcElements ? 0 : src[srcIndex];

				srcIndex += Cols_C;
			}
		}
	};

	template<typename T, size_t N>
	constexpr Matrix<T, N, N>& identity(Matrix<T, N, N>& matrix)
	{
		memset(&matrix[0][0], 0, sizeof(T) * N * N);

		for (size_t i = 0; i < N; i++)
		{
			matrix[i][i] = 1;
		}

		return matrix;
	}

	template<typename T, size_t N>
	constexpr Matrix<T, N, N>& translation(Matrix<T, N, N>& matrix, const Vector<T, (N - 1)>& translation)
	{
		memset(&matrix[0][0], 0, sizeof(T) * N * N);

		for (size_t i = 0; i < N; i++)
		{
			matrix[i][i] = 1;
		}

		memcpy_s(&matrix[N - 1], sizeof(T) * N, &translation, sizeof(T) * (N - 1));

		return matrix;
	}

	template<typename T, size_t N>
	constexpr Matrix<T, N, N>& scale(Matrix<T, N, N>& matrix, const Vector<T, N>& scale)
	{
		memset(&matrix[0][0], 0, sizeof(T) * N * N);

		for (size_t i = 0; i < N; i++)
		{
			matrix[i][i] = scale[i];
		}

		return matrix;
	}

	template<typename T>
	constexpr Matrix<T, 4, 4> perspective(T aspectRatio, T fieldOfView, T nearPlane, T farPlane)
	{
		T f = T(1) / std::tan(T(0.5) * fieldOfView);
		T zRange = nearPlane - farPlane;

		Matrix<T, 4, 4> result;

		result[0][0] = f / aspectRatio; result[1][0] = T(0); result[2][0] = T(0);				result[3][0] = T(0);
		result[0][1] = T(0);			result[1][1] = -f;   result[2][1] = T(0);				result[3][1] = T(0);
		result[0][2] = T(0);			result[1][2] = T(0); result[2][2] = farPlane / zRange;  result[3][2] = (nearPlane * farPlane) / zRange;
		result[0][3] = T(0);			result[1][3] = T(0); result[2][3] = T(-1);				result[3][3] = T(0);


		return result;
	}

	template<typename T>
	constexpr Matrix<T, 4, 4> perspectiveOGL(T aspectRatio, T fieldOfView, T nearPlane, T farPlane)
	{
		T f = T(1) / std::tan(T(0.5) * fieldOfView);
		T zRange = nearPlane - farPlane;

		Matrix<T, 4, 4> result;

		result[0][0] = f / aspectRatio; result[1][0] = T(0); result[2][0] = T(0);								result[3][0] = T(0);
		result[0][1] = T(0);			result[1][1] = f;    result[2][1] = T(0);								result[3][1] = T(0);
		result[0][2] = T(0);			result[1][2] = T(0); result[2][2] = (-nearPlane - farPlane) / zRange;   result[3][2] = (2 * nearPlane * farPlane) / zRange;
		result[0][3] = T(0);			result[1][3] = T(0); result[2][3] = T(1);								result[3][3] = T(0);


		return result;
	}

	template<typename T>
	constexpr Matrix<T, 4, 4> orthographic(T left, T right, T bottom, T top, T near, T far)
	{
		Matrix<T, 4, 4> result;

		result[0][0] = T(2) / (right - left); result[1][0] = T(0);					  result[2][0] = T(0);				  result[3][0] = (right + left) / (left - right);
		result[0][1] = T(0);				  result[1][1] = T(2) / (bottom - top);   result[2][1] = T(0);				  result[3][1] = (bottom + top) / (top - bottom);
		result[0][2] = T(0);				  result[1][2] = T(0);					  result[2][2] = T(1) / (near - far); result[3][2] = near / (near - far);
		result[0][3] = T(0);				  result[1][3] = T(0);					  result[2][3] = T(0);				  result[3][3] = T(1);

		return result;
	}

	template<typename T>
	constexpr Matrix<T, 4, 4> orthographicOGL(T left, T right, T bottom, T top, T near, T far)
	{
		Matrix<T, 4, 4> result;

		result[0][0] = T(2) / (right - left); result[1][0] = T(0);					  result[2][0] = T(0);				  result[3][0] = (right + left) / (left - right);
		result[0][1] = T(0);				  result[1][1] = T(2) / (top - bottom);   result[2][1] = T(0);				  result[3][1] = (bottom + top) / (bottom - top);
		result[0][2] = T(0);				  result[1][2] = T(0);					  result[2][2] = T(2) / (near - far); result[3][2] = (far + near) / (near - far);
		result[0][3] = T(0);				  result[1][3] = T(0);					  result[2][3] = T(0);				  result[3][3] = T(1);

		return result;
	}
}