#include "../ostream.hpp"
#include <cstdint>
#include <string>
#include <cstdio>
#include <stdexcept>

class FileOutputStream : public OutputStream {

private:
	FILE* outputFile;


public:

	std::size_t write(const void* dst, std::size_t size, std::size_t length);
	OutputStream& outputSeek(uint64_t offset, StreamPosition position);
	uint64_t 	  outputTell();

	void close() { std::fclose(outputFile); outputOpen = false; }

	FileOutputStream(std::string filepath);
	~FileOutputStream();
};