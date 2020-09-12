#include "nifreader.hpp"

NifReader::NifReader(const std::string& filePath)
{
    file = getFileProvider().openFile(filePath);
    //file = std::fopen(filePath, "rb");
    if (file == NULL) {
        throw std::invalid_argument(std::string("Cannot open file: ") + std::string(filePath));
    }
}

NifReader::~NifReader()
{
    /*for(unsigned int i = 0; i < numBlockTypes; i++) {
		delete[] blockTypes[i];
	}
	for(unsigned int i = 0; i < numStrings; i++) {
		delete[] strings[i];
	}
	delete[] strings;*/
    getFileProvider().closeFile(file);
    delete file;
    //std::fclose(file);
}

void NifReader::readNifHeader()
{
    if (skipTerminatedString('\n') == -1) { //Skip header version string.
        throw std::invalid_argument(std::string("Invalid file"));
    }
    file->read(&version, sizeof(uint32_t), 1);
    file->inputSeek(sizeof(uint8_t), StreamPosition::cur); //Skip endianness.
    file->read(&userVersion, sizeof(uint32_t), 1);
    file->read(&numBlocks, sizeof(uint32_t), 1);

    //start BS header
    file->read(&bsVersion, sizeof(uint32_t), 1);
    skipSizedString(); // Skip author name.
    if (bsVersion > 130) {
        file->inputSeek(sizeof(uint32_t), StreamPosition::cur); // Skip unknown int.
    }
    skipSizedString(); // Skip process script name.
    skipSizedString(); // Skip export script name.
    if (bsVersion == 130) {
        skipSizedString(); // Skip "max filepath"
    }
    //end BS header
    file->read(&numBlockTypes, sizeof(uint16_t), 1);
    blockTypes.resize(numBlockTypes);

    for (unsigned int i = 0; i < numBlockTypes; i++) {
        blockTypes[i] = loadSizedString();
    }

    blockTypeIndices.resize(numBlocks);
    file->read(&blockTypeIndices[0], sizeof(int16_t), numBlocks);

    std::vector<uint32_t> blockSizes(numBlocks); // Temporary.
    file->read(&blockSizes[0], sizeof(uint32_t), numBlocks);

    file->read(&numStrings, sizeof(uint32_t), 1);
    file->read(&maxStringLength, sizeof(uint32_t), 1);

    strings.resize(numStrings);
    for (unsigned int i = 0; i < numStrings; i++) {
        strings[i] = loadSizedString();
        //loadSizedString(&strings[i * (maxStringLength + 1)]);
    }

    uint32_t numGroups;
    file->read(&numGroups, sizeof(uint32_t), 1);
    file->inputSeek(sizeof(uint32_t) * numGroups, StreamPosition::cur); // Skip groups for now.
    //delete[] blockSizes;
}

NiObject* NifReader::readBlock(uint32_t index)
{
    const char* type = blockTypes[blockTypeIndices[index]];
    //typename std::unordered_map<std::string, NiObject* (*)(FILE*)>::const_iterator mapIterator = NiFactory::creatorMap.find(type);
    auto mapIterator = NiFactory::creatorMap.find(type);
    if (mapIterator == NiFactory::creatorMap.end()) {
        throw std::runtime_error(std::string("Cannot find factory function for: ") + std::string(type));
    }

    NiObject* (*factory)(NifReader&) = mapIterator->second;
    return factory(*this);
}

int NifReader::skipTerminatedString(char sep)
{
    if (file == NULL) {
        return -1;
    }
    while (true) {
        int c = file->getc();
        if (c == EOF || c == sep) {
            break;
        }
    }
    if (file->isEnded()) {
        return -1;
    }
    return 0;
}

void NifReader::skipSizedString()
{
    uint8_t length;
    file->read(&length, sizeof(uint8_t), 1);
    file->inputSeek(length, StreamPosition::cur);
}

char* NifReader::loadSizedString()
{
    uint32_t length;
    file->read(&length, sizeof(uint32_t), 1);

    char* dst = new char[length + 1];
    file->read(dst, sizeof(uint8_t), length);
    dst[length] = '\0';
    return dst;
}

int NifReader::read(void* dst, uint32_t size, uint32_t length)
{
    return file->read(dst, size, length);
}

char* NifReader::readIndexedString()
{

    uint32_t index;
    file->read(&index, sizeof(uint32_t), 1);

    if (static_cast<int32_t>(index) == -1) {
        return NULL;
    }
    char* dst = new char[maxStringLength + 1];
    std::strcpy(dst, strings[index]);
    return dst;
}
