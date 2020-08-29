#pragma once
#include "utils.hpp"
#include <cstddef>
#include <cstdint>
#include <stdexcept>

/*
	An input stream.
	The input stream supports reading a source, seeking across the source,
	and showing the current location.
*/
class InputStream {

protected:

	// Number of objects read in last read.
	std::size_t lastRead = 0;

	// Is the stream open.
	bool inputOpen = true;


	void checkState() {
		if(!inputOpen) {
			throw std::logic_error("Stream is closed!");
		}
	}

public:

	/*
		Reads length objects of size size from the stream, writing them into dst.
	*/
	virtual std::size_t	 read(void* dst, std::size_t size, std::size_t length) = 0;

	/*
		Sets the stream location to the given location, from the beginning.
	*/
	InputStream& 	     inputSeek(uint64_t loc);

	/*
		Sets the stream location to the given offset depending on the position value.
	*/
	virtual InputStream& inputSeek(uint64_t offset, StreamPosition position) = 0;
	
	/*
		Gets the current location.
	*/
	virtual uint64_t 	 inputTell() = 0;

	std::size_t getLastRead() { return lastRead; }

	/*
		Returns true if the end of the stream has been reached.
	*/
	virtual bool isEnded() = 0;

	virtual int getc() = 0;

	/*
		Closes the stream.
	*/
	virtual void close() = 0;

	virtual ~InputStream() {}

	InputStream() {}
	InputStream(InputStream& is) = delete;
};