#include "nifreader.hpp"

NifReader::NifReader(std::vector<uint8_t> nifFileData) : streamBuffer(nifFileData), file(&streamBuffer)
{}

NifReader::~NifReader()
{
}

void NifReader::readNifHeader()
{
    if (skipTerminatedString('\n') == -1) { //Skip header version string.
        throw std::runtime_error(std::string("Invalid file"));
    }
    file.read((char*)&version, sizeof(uint32_t));
    file.seekg(sizeof(uint8_t), std::ios::cur); //Skip endianness.
    file.read((char*)&userVersion, sizeof(uint32_t));
    file.read((char*)&numBlocks, sizeof(uint32_t));

    //start BS header
    file.read((char*)&bsVersion, sizeof(uint32_t));
    skipSizedString(); // Skip author name.
    if (bsVersion > 130) {
        file.seekg(sizeof(uint32_t), std::ios::cur); // Skip unknown int.
    }
    skipSizedString(); // Skip process script name.
    skipSizedString(); // Skip export script name.
    if (bsVersion == 130) {
        skipSizedString(); // Skip "max filepath"
    }
    //end BS header
    file.read((char*)&numBlockTypes, sizeof(uint16_t));
    blockTypes.resize(numBlockTypes);

    for (unsigned int i = 0; i < numBlockTypes; i++) {
        blockTypes[i] = loadSizedString();
    }

    blockTypeIndices.resize(numBlocks);
    file.read((char*)&blockTypeIndices[0], sizeof(int16_t) * numBlocks);

    std::vector<uint32_t> blockSizes(numBlocks); // Temporary.
    file.read((char*)&blockSizes[0], sizeof(uint32_t) * numBlocks);

    file.read((char*)&numStrings, sizeof(uint32_t));
    file.read((char*)&maxStringLength, sizeof(uint32_t));

    strings.resize(numStrings);
    for (unsigned int i = 0; i < numStrings; i++) {
        strings[i] = loadSizedString();
        //loadSizedString(&strings[i * (maxStringLength + 1)]);
    }

    uint32_t numGroups;
    file.read((char*)&numGroups, sizeof(uint32_t));
    file.seekg(sizeof(uint32_t) * numGroups, std::ios::cur); // Skip groups for now.
    //delete[] blockSizes;
}

NiObject* NifReader::readBlock(uint32_t index)
{
    std::string type = blockTypes[blockTypeIndices[index]];

    auto mapIterator = NiFactory::creatorMap.find(type.c_str());
    if (mapIterator == NiFactory::creatorMap.end()) {
        throw std::runtime_error(std::string("Cannot find factory function for: ") + type);
    }

    NiObject* (*factory)(NifReader&) = mapIterator->second;
    return factory(*this);
}

int NifReader::skipTerminatedString(char sep)
{
    while (true) {
        int c = file.get();
        if (c == EOF || c == sep) {
            break;
        }
    }
    if (file.eof()) {
        return -1;
    }
    return 0;
}

void NifReader::skipSizedString()
{
    uint8_t length;
    file.read((char*)&length, sizeof(uint8_t));
    file.seekg(length, std::ios::cur);
}

std::string NifReader::loadSizedString()
{   
    std::string loadedString;
    uint32_t length;
    file.read((char*)&length, sizeof(uint32_t));

    loadedString.resize(length + 1);
    //char* dst = new char[length + 1];
    file.read(&loadedString.data()[0], sizeof(uint8_t) * length);
    loadedString[length] = '\0';
    return loadedString;
}

void NifReader::read(void* dst, uint32_t size, uint32_t length)
{
    file.read((char*) dst, size * length);
}

std::string NifReader::readIndexedString()
{
    uint32_t index;
    file.read((char*)&index, sizeof(uint32_t));

    if (static_cast<int32_t>(index) == -1) {
        return std::string();
    }

    return strings[index];
}
