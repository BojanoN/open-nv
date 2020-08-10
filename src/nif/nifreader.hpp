#pragma once

class NifReader {

private:
	FILE* file;

	//Major engine version
	uint32_t version = 0x04000002;

	//Minor engine version
	uint32_t userVersion;

	//Bethesda extra version
	uint32_t bsVersion;

	// Data about blocks
	uint32_t numBlocks;
	uint32_t numBlockTypes;
	char** blockTypes;

	// Strings of the Nif file
	uint32_t numStrings;
	uint32_t maxStringLength;
	char* strings;

public:

	NifReader(const char* filePath);
	~NifReader();

	void readNifHeader(NifData* data);

	int read(void* dst, uint32_t size, uint32_t length);
	void readIndexedString(char* dst);

	inline uint32_t getNumBlocks() {
		return numBlocks;
	}
};