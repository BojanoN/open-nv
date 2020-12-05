#pragma once
#include <cstdint>

namespace Types {

template <typename ValueType>
struct ErrorPair {

	uint32_t error;
	ValueType value;

	ErrorPair(uint32_t error, ValueType value) : error{error}, value{value} {}
	ErrorPair(uint32_t error) : error{error} {}

	bool fail() {
		return error != 0;
	}
};


}; // namespace Types