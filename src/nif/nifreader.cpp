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

void NifReader::readNifHeader(NifData* data) {
	if(skipTerminatedString('\n') == -1) { //Skip header version string.
		throw std::invalid_argument(std::string("Invalid file: ") + std::string(filePath))
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

	std::fread(&numBlockTypes, sizeof(uint32_t), 1, file);
	blockTypes = new char*[numBlockTypes];

	for(unsigned int i = 0; i < numBlockTypes; i++) {
		loadSizedString(blockTypes[i]);
	}

	uint32_t* blockSizes = new uint32_t[numBlocks]; // Temporary.
	std::fread(blockSizes, sizeof(uint32_t), numBlocks, file);

	std::fread(&numStrings, sizeof(uint32_t), 1, file);
	std::fread(&maxStringLength, sizeof(uint32_t), 1, file);

	strings = new char[numStrings * (maxStringLength + 1)];
	for(unsigned int i = 0; i < numStrings; i++) {
		loadSizedString(&strings[i * (maxStringLength + 1)]);
	}

	uint32_t numGroups;
	std::fread(&numGroups, sizeof(uint32_t), 1, file);
	std::fseek(file, sizeof(uint32_t) * numGroups, file); // Skip groups for now.

	delete[] blockSizes;
}


void NifReader::readBlock(uint32_t index, NiObject* dst) {
	const char* type = blockTypes[blockTypeIndex[index]];
	typename std::unordered_map<std::string, NiObject* (*)(FILE*)>::const_iterator mapIterator = NiFactory::creatorMap.find(type);
	if(mapIterator == NiFunctionMaps::creatorMap.end()) {
			std::printf("Cannot find factory function for: %s\n", type);
			return NULL;
	}

	NiObject* (*factory)(FILE*) = mapIterator->second;
	dst = factory(this);
}


int NifReader::skipTerminatedString(char sep) { 
	if(file == NULL) {
		return -1;
	}
	while(int c = std::getc(file); c != EOF && c != sep);
	if(std::feof(file)) {
		return -1;
	}
}

void NifReader::skipSizedString() {
	uint8_t length;
	std::fread(&length, sizeof(uint8_t), 1, file);
	std::fseek(file, length, SEEK_CUR);
}

void NifReader::loadSizedString(char* dst) {
	if (dst != NULL) {
		delete[] dst;
	}
	uint32_t length;
	std::fread(&length, sizeof(uint32_t), 1, file);
	dst = new char[length + 1];
	std::fread(dst, sizeof(uint8_t), length, file);
	dst[length] = '\0';
}


int NifReader::read(void* dst, uint32_t size, uint32_t length) {
	std::fread(dst, size, length, file);
}

void NifReader::readIndexedString(char* dst) {
	if(dst == NULL) {
		dst = new char[maxStringLength];
	}
	uint32_t index;
	std::fread(&index, sizeof(uint32_t), 1, file);
	std::memcpy(dst, strings[index * (maxStringLength + 1)], maxStringLength);
}