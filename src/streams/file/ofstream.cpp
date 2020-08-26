#include "ofstream.hpp"

FileOutputStream::FileOutputStream(std::string filepath) {
	this->outputFile = std::fopen(filepath.c_str(), "wb");

	if(this->outputFile == NULL) {
		throw std::invalid_argument(std::string("Cannot open file: ") + filepath);
	}
}

FileOutputStream::~FileOutputStream() {
	std::fclose(this->outputFile);
}

std::size_t FileOutputStream::write(const void* src, std::size_t size, std::size_t length) {
	this->checkState();
	this->lastWritten = std::fwrite(src, size, length, outputFile);
	return this->lastWritten;
}

OutputStream& FileOutputStream::outputSeek(uint64_t offset, StreamPosition position) {
	switch(position) {
	
	case StreamPosition::beg:
		std::fseek(outputFile, offset, SEEK_SET);
		break;
	case StreamPosition::cur:
		std::fseek(outputFile, offset, SEEK_CUR);
		break;	
	case StreamPosition::end:
		std::fseek(outputFile, offset, SEEK_END);
		break;
	}

	return *this;
}

uint64_t FileOutputStream::outputTell() {
	return std::ftell(outputFile);
}