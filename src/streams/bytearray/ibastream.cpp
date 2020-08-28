#include "ibastream.hpp"

ByteArrayInputStream::ByteArrayInputStream(std::vector<uint8_t>* byteArray) : pos{0} {
	//std::copy(byteArray.begin(), byteArray.end(), std::back_inserter(this->array));
	this->array = byteArray;
}

std::size_t ByteArrayInputStream::read(void* _dst, std::size_t size, std::size_t length) {
	this->checkState();
	uint8_t* dst = reinterpret_cast<uint8_t*>(_dst);

	std::size_t read = length; // Presume
	for(unsigned int i = 0; i < size * length; i++) {
		
		if(pos == array->size()) {
			read = i / size;
			break;
		}

		dst[i] = (*(array))[pos++];
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
		pos = array->size() - offset;
		break;
	}
	return *this;
}

uint64_t ByteArrayInputStream::inputTell() {
	return pos;
}

bool ByteArrayInputStream::isEnded() {
	return pos == array->size();
}