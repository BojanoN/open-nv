#pragma once
#include <cstdint>

namespace Err {

typedef uint32_t error_t;

enum ErrorCodes : error_t {
	codeSuccess = 0,
	codeInvalidArgument = 1,
	codeKeyNotFound = 2,
	codeTypeError,
	codeNullPointerError,
	codeNumberFormatError,
	codeOutOfRange,
	codeFloatInfinity,
	codeFloatNaN,
	codeMemoryError,
	codeFileNotFound,
	codeIOError,
};


class Error {

	error_t value;
public:

	Error(error_t value) : value{value} {}

	bool fail() { return value != codeSuccess; }
	bool success() { return !fail(); }

};

inline Error Success(codeSuccess);
inline Error InvalidArgument(codeInvalidArgument);
inline Error KeyNotFound(codeKeyNotFound);
inline Error TypeError(codeTypeError);
inline Error NullPointerError(codeNullPointerError);
inline Error NumberFormatError(codeNumberFormatError);
inline Error OutOfRange(codeOutOfRange);
inline Error FloatInfinity(codeFloatInfinity);
inline Error FloatNaN(codeFloatNaN);
inline Error MemoryError(codeMemoryError);
inline Error FileNotFound(codeFileNotFound);
inline Error IOError(codeIOError);

};