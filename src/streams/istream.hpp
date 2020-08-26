#pragma once
#include "utils.hpp"
#include <cstddef>
#include <cstdint>
#include <stdexcept>

class InputStream {

protected:

	std::size_t lastRead = 0;
	bool inputOpen = true;

	void checkState() {
		if(!inputOpen) {
			throw std::logic_error("Stream is closed!");
		}
	}

public:

	virtual std::size_t			 read(void* dst, std::size_t size, std::size_t length) = 0;
	InputStream& 	     inputSeek(uint64_t loc);
	virtual InputStream& inputSeek(uint64_t offset, StreamPosition position) = 0;
	virtual uint64_t 	 inputTell() = 0;

	std::size_t getLastRead() { return lastRead; }
	virtual bool isEnded() = 0;
	virtual void close() = 0;

	virtual ~InputStream() {}
};