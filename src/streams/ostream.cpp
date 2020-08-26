#include "ostream.hpp"

OutputStream& OutputStream::outputSeek(uint64_t loc) {
	return outputSeek(loc, StreamPosition::beg);
}