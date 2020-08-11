#include "nifreader.hpp"

NifReader::NifReader(const char* filePath) {
	file = std::fopen(filePath, "rb");
	if(file == NULL) {
		throw std::invalid_argument(std::string("Cannot open file: ") + std::string(filePath));
	}
}

NifReader::~NifReader() {
	for(unsigned int i = 0; i < numBlockTypes; i++) {
		delete[] blockTypes[i];
	}
	delete[] blockTypes;
	delete[] strings;
	std::fclose(file);
}

void NifReader::readNifHeader() {
	if(skipTerminatedString('\n') == -1) { //Skip header version string.
		throw std::invalid_argument(std::string("Invalid file"));
	}
	std::fread(&version, sizeof(uint32_t), 1, file);
	std::fseek(file, sizeof(uint8_t), SEEK_CUR); //Skip endianness.
	std::fread(&userVersion, sizeof(uint32_t), 1, file);
	std::fread(&numBlocks, sizeof(uint32_t), 1, file);

	//start BS header
	std::fread(&bsVersion, sizeof(uint32_t), 1, file);
	skipSizedString(); // Skip author name.
	if(bsVersion > 130) {
		std::fseek(file, sizeof(uint32_t), SEEK_CUR); // Skip unknown int.
	}
	skipSizedString(); // Skip process script name.
	skipSizedString(); // Skip export script name.
	if(bsVersion == 130) {
		skipSizedString(); // Skip "max filepath"
	}
	//end BS header

	std::fread(&numBlockTypes, sizeof(uint16_t), 1, file);
	blockTypes = new char*[numBlockTypes];

	for(unsigned int i = 0; i < numBlockTypes; i++) {
		blockTypes[i] = loadSizedString();
	}

	blockTypeIndices = new int16_t[numBlocks];
	std::fread(blockTypeIndices, sizeof(int16_t), numBlocks, file);

	uint32_t* blockSizes = new uint32_t[numBlocks]; // Temporary.
	std::fread(blockSizes, sizeof(uint32_t), numBlocks, file);

	std::fread(&numStrings, sizeof(uint32_t), 1, file);
	std::fread(&maxStringLength, sizeof(uint32_t), 1, file);

	strings = new char*[numStrings];
	for(unsigned int i = 0; i < numStrings; i++) {
		strings[i] = loadSizedString();
		//loadSizedString(&strings[i * (maxStringLength + 1)]);
	}

	uint32_t numGroups;
	std::fread(&numGroups, sizeof(uint32_t), 1, file);
	std::fseek(file, sizeof(uint32_t) * numGroups, SEEK_CUR); // Skip groups for now.

	delete[] blockSizes;
}


NiObject* NifReader::readBlock(uint32_t index) {
	const char* type = blockTypes[blockTypeIndices[index]];
	//typename std::unordered_map<std::string, NiObject* (*)(FILE*)>::const_iterator mapIterator = NiFactory::creatorMap.find(type);
	auto mapIterator = NiFactory::creatorMap.find(type);
	if(mapIterator == NiFactory::creatorMap.end()) {
			std::printf("Cannot find factory function for: %s\n", type);
			return NULL;
	}

	NiObject* (*factory)(NifReader&) = mapIterator->second;
	return factory(*this);
}


int NifReader::skipTerminatedString(char sep) { 
	if(file == NULL) {
		return -1;
	}
	while(true) {
		int c = std::getc(file);
		if(c == EOF || c == sep) {
			break;
		}
	}
	if(std::feof(file)) {
		return -1;
	}
	return 0;
}

void NifReader::skipSizedString() {
	uint8_t length;
	std::fread(&length, sizeof(uint8_t), 1, file);
	std::fseek(file, length, SEEK_CUR);
}

char* NifReader::loadSizedString() {
	uint32_t length;
	std::fread(&length, sizeof(uint32_t), 1, file);
	char* dst = new char[length + 1];
	std::fread(dst, sizeof(uint8_t), length, file);
	dst[length] = '\0';
	return dst;
}


int NifReader::read(void* dst, uint32_t size, uint32_t length) {
	return std::fread(dst, size, length, file);
}

char* NifReader::readIndexedString() {
	char* dst = new char[maxStringLength];
	uint32_t index;
	std::fread(&index, sizeof(uint32_t), 1, file);
	std::strcpy(dst, strings[index]);
	return dst;
}