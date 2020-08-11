#pragma once

//#include "nifdata.hpp"

#include "nifactory.hpp"
#include "file/reader.hpp"
#include "streams/istream.hpp"

#include <cstdio>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <cstring>
#include <unordered_map>


class NiObject;

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
	uint16_t numBlockTypes;

	std::vector<int16_t> blockTypeIndices;
	std::vector<char*> blockTypes;
	// Strings of the Nif file
	uint32_t numStrings;
	uint32_t maxStringLength;
	char** strings;

public:

	NifReader(const char* filePath);
	~NifReader();

	void readNifHeader();

	int read(void* dst, uint32_t size, uint32_t length);
	char* readIndexedString();
	int skipTerminatedString(char sep); 
	void skipSizedString();
	char* loadSizedString();
	//std::shared_ptr<NiObject> readBlock(uint32_t index);
	NiObject* readBlock(uint32_t index);
	//NiObject* readNifTree();

	inline uint32_t getNumBlocks() {
		return numBlocks;
	}
};