#pragma once

#include <type_traits>
#include <memory>

template<typename T, size_t TCount>
class Vector {
	static_assert(std::is_arithmetic<T>::value, "Type must be number");

	Vector() {
		data = std::make_unique(sizeof(T) * TCount);
	}

private:
	std::unique_ptr<T> data;
};