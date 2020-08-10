#include <string>
#include <cstdio>
#include <cstdlib>

class NifData {

private:
	uint32_t numBlocks;
	NiObject** blocks; // for now

public:

	NifData(const char* filePath);
	~NifData();
	//char* getString(int index);
	//int readData(uint32_t size);
	NiObject* getBlock(uint32_t index) { return blocks[index]; }
};