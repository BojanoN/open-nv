#include "istream.hpp"

InputStream& InputStream::inputSeek(uint64_t loc) {
	return inputSeek(loc, StreamPosition::beg);
}