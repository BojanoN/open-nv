#pragma once
#include "error/error.hpp"
#include <cstdint>

namespace Types {

using Err::Error;

template <typename ValueType>
struct ErrorPair {

	Error error;
	ValueType value;

	ErrorPair() {}
	ErrorPair(Error error, ValueType value) : error{error}, value{value} {}
	ErrorPair(Error error) : error{error} {}

	bool fail() {
		return error.fail();
	}

	bool success() {
		return error.success();
	}
};


}; // namespace Types