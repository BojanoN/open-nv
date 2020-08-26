#include "../istream.hpp"
#include <cstdint>
#include <vector>
#include <stdexcept>

class ByteArrayInputStream : public InputStream {

private:

	std::vector<uint8_t> array;
	uint64_t pos;

public:

	std::string  read(void* dst, std::size_t size, std::size_t length);
	InputStream& inputSeek(uint64_t offset, StreamPosition position);
	uint64_t 	 inputTell();

	bool isEnded();
	void close() { inputOpen = false; }

	ByteArrayInputStream(std::vector<uint8_t> array);
	~ByteArrayInputStream() {}
};