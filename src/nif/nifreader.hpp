#pragma once

#include "nifactory.hpp"
#include "file/reader.hpp"
#include "util/bytestream.hpp"

#include <cstdio>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <cstring>
#include <unordered_map>
#include <sstream>


class NiObject;

class NifReader : File::Reader {

private:

	ByteBuffer streamBuffer;
	std::istream file;

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
	
	std::vector<char*> strings;

public:

	NifReader(std::vector<uint8_t> nifFileData);
	~NifReader();

	void readNifHeader();

	void read(void* dst, uint32_t size, uint32_t length);
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