#include "ifstream.hpp"

FileInputStream::FileInputStream(std::string filepath) {
	this->inputFile = std::fopen(filepath.c_str(), "rb");

	if(this->inputFile == NULL) {
		throw std::invalid_argument(std::string("Cannot open inputFile") + filepath);
	}
}

FileInputStream::~FileInputStream() {
	if(inputOpen) {
		std::fclose(this->inputFile);
	}
}

std::size_t FileInputStream::read(void* dst, std::size_t size, std::size_t length) {
	this->checkState();
	this->lastRead = std::fread(dst, size, length, inputFile);
	return this->lastRead;
}

InputStream& FileInputStream::inputSeek(uint64_t offset, StreamPosition position) {
	switch(position) {
	
	case StreamPosition::beg:
		std::fseek(inputFile, offset, SEEK_SET);
		break;
	case StreamPosition::cur:
		std::fseek(inputFile, offset, SEEK_CUR);
		break;	
	case StreamPosition::end:
		std::fseek(inputFile, offset, SEEK_END);
		break;
	}

	return *this;
}

uint64_t FileInputStream::inputTell() {
	return std::ftell(inputFile);
}

bool FileInputStream::isEnded() {
	return std::feof(this->inputFile);
}