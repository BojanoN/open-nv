#pragma once

#include "record.hpp"
#include "subrecord.hpp"
#include <cstdio>
#include <string>
#include <unordered_map>

namespace ESM {

class ESMReader {
public:
    ESMReader(std::string& path)
    {

        this->file = std::fopen(path.c_str(), "rb");
        if (this->file == NULL) {
            exit(1);
        }
        std::fseek(this->file, 0, SEEK_END);

        ssize_t size   = std::ftell(this->file);
        this->fileSize = size;
        this->fileLeft = size;

        std::fseek(this->file, 0, SEEK_SET);
    };
    ~ESMReader() {};

    bool hasMoreSubrecords() { return this->recordLeft > 0; };
    bool hasMoreRecords() { return this->fileLeft > 0; };
    void skipRecord();

    RecordHeader&    getCurrentRecord();
    SubrecordHeader& getCurrentSubrecord();
    uint32_t         getCurrentRecordType();
    uint32_t         getCurrentSubrecordType();

    void readNextSubrecord();
    void readNextRecord();

    uint32_t peekNextType();

    template <typename T>
    void readGeneric(T* dest);

    std::string* readCstringSubrecord();

private:
    FILE*   file;
    ssize_t fileSize;

    ssize_t fileLeft;
    ssize_t recordLeft;

    RecordHeader    currentRecordHead;
    SubrecordHeader currentSubrecordHead;
};
}
