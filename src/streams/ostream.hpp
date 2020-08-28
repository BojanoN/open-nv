#pragma once
#include "utils.hpp"
#include <cstddef>
#include <cstdint>
#include <stdexcept>

class OutputStream {

protected:

	std::size_t lastWritten = 0;
	bool outputOpen = true;

	void checkState() {
		if(!outputOpen) {
			throw std::logic_error("Stream is closed!");
		}
	}

public:

	virtual std::size_t write(const void* dst, std::size_t size, std::size_t length) = 0;
	OutputStream& 	      outputSeek(uint64_t loc);
	virtual OutputStream& outputSeek(uint64_t offset, StreamPosition position) = 0;
	virtual uint64_t 	  outputTell() = 0;
	virtual void close() = 0;

	virtual ~OutputStream() {}
};