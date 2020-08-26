#include "ibastream.hpp"

ByteArrayInputStream::ByteArrayInputStream(std::vector<uint8_t> array) {
	this->array = array;
}

std::size_t ByteArrayInputStream::read(void* dst, std::size_t size, std::size_t length) {
	this->checkState();
	dst = reinterpret_cast<uint8_t*>(dst);

	std::size_t read = length; // Presume
	for(unsigned int i = 0; i < size * length; i++) {
		
		try {	
			dst[i] = array.at(pos++);

		} catch (std::out_of_range& e) {
			read = i / size;
			
			break;
		}
	}
	this->lastRead = read;

	return this->lastRead;
}

InputStream& ByteArrayInputStream::inputSeek(uint64_t offset, StreamPosition position) {
	
	switch(position) {
	
	case StreamPosition::beg:
		pos = offset;
		break;
	case StreamPosition::cur:
		pos += offset;
		break;
	case StreamPosition::end:
		pos = array.size() - offset;
		break;
	}
	return *this;
}

uint64_t ByteArrayInputStream::inputTell() {
	return pos;
}

bool ByteArrayInputStream::isEnded() {
	return pos == array.size();
}