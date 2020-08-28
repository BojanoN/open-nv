#include "../istream.hpp"
#include <cstdint>
#include <string>
#include <cstdio>
#include <stdexcept>

/*
	FileInputStream supports reading from a file.
*/
class FileInputStream : public InputStream {

private:
	FILE* inputFile;


public:

	std::size_t  read(void* dst,  std::size_t size, std::size_t length);
	InputStream& inputSeek(uint64_t offset, StreamPosition position);
	uint64_t 	 inputTell();

	bool isEnded();
	void close() { std::fclose(inputFile); inputOpen = false; }

	FileInputStream(std::string filepath);
	~FileInputStream();
};