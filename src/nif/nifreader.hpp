#pragma once

#include "file/reader.hpp"
#include "nifactory.hpp"
#include "util/bytestream.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

struct NiObject;

class NifReader : File::Reader {

private:
    ByteBuffer   streamBuffer;
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

    std::vector<int16_t>     blockTypeIndices;
    std::vector<std::string> blockTypes;
    // Strings of the Nif file
    uint32_t numStrings;
    uint32_t maxStringLength;

    std::vector<std::string> strings;

public:
    NifReader(std::vector<uint8_t> nifFileData);
    ~NifReader();

    void readNifHeader();

    void        read(void* dst, uint32_t size, uint32_t length);
    std::string readIndexedString();
    int         skipTerminatedString(char sep);
    void        skipSizedString();
    std::string loadSizedString();
    //std::shared_ptr<NiObject> readBlock(uint32_t index);
    NiObject* readBlock(uint32_t index);
    //NiObject* readNifTree();

    inline uint32_t getNumBlocks()
    {
        return numBlocks;
    }
};
