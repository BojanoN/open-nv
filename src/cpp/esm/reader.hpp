#pragma once

#include "headers.hpp"
#include "logc/log.h"
#include "types.hpp"
#include "utils.hpp"

#include <cassert>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace ESM {

class ESMReader {
public:
    ESMReader(std::string& path)
        : fileName { path }
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
    ~ESMReader()
    {
        if (this->file)
            std::fclose(this->file);
    };

    bool hasMoreSubrecords() { return std::ftell(this->file) < endOfRecord; }
    bool hasMoreRecordsInGroup() { return std::ftell(this->file) < endOfGroup; }
    bool hasMoreBytes() { return std::ftell(this->file) < fileSize; }
    void skipRecord();
    void skipGroup();

    RecordHeader&    getCurrentRecord();
    SubrecordHeader& getCurrentSubrecord();
    uint32_t         recordType();
    uint32_t         subrecordType();
    uint32_t         groupLabel();
    int32_t          groupType();

    uint32_t recordFlags();
    formid   recordId();

    uint16_t subrecordSize() { return currentSubrecordHead.dataSize; }

    void readNextSubrecordHeader();
    void readNextRecordHeader();
    void readNextGroupHeader();
    void readNextHeader();

    void checkSubrecordHeader(ESMType type);

    uint32_t peekNextType();
    void     rewind(ssize_t size);

    std::string getFileName() { return this->fileName; };

    template <typename T>
    void readSubrecord(T& subrecValue);

    void readStringSubrecord(std::string& subrecString);

    template <typename T>
    void readArraySubrecord(std::vector<T>& array);

    template <typename T>
    int readRawData(T& value);

    template <typename T>
    int readRawArray(T* array, ssize_t length);

    template <typename T>
    void readFixedArraySubrecord(T* array);

    void reportError(std::string err);

private:
    std::FILE*  file;
    ssize_t     fileSize;
    std::string fileName;

    ssize_t fileLeft;
    ssize_t recordLeft;
    ssize_t groupLeft;

    ssize_t endOfRecord;
    ssize_t endOfGroup;
    ssize_t endOfSubrecord;

    GroupHeader     currentGroupHead;
    RecordHeader    currentRecordHead;
    SubrecordHeader currentSubrecordHead;
};

template <typename T>
void ESMReader::readSubrecord(T& subrecValue)
{
    int actual = std::fread(&subrecValue, currentSubrecordHead.dataSize, 1, this->file);
    actual *= currentSubrecordHead.dataSize;
    if (actual != sizeof(T)) {
        std::stringstream s;
        s << "Expected to read " << sizeof(T) << " bytes, actually read " << actual << " bytes,\n";
        s << " in subrecord " << Util::typeValueToName(currentSubrecordHead.type) << ", in record " << Util::typeValueToName(currentRecordHead.type) << " at 0x" << std::hex << std::ftell(this->file);
        log_fatal(s.str().c_str());
        throw std::runtime_error("Read mismatch!");
    }
}

template <typename T>
int ESMReader::readRawData(T& value)
{
    return std::fread(&value, sizeof(T), 1, this->file);
}

template <typename T>
int ESMReader::readRawArray(T* array, ssize_t length)
{
    return std::fread(array, sizeof(T), length, this->file);
}

template <typename T>
void ESMReader::readArraySubrecord(std::vector<T>& array)
{
    array.resize(currentSubrecordHead.dataSize / sizeof(T));
    int actual = std::fread(&array[0], sizeof(T), currentSubrecordHead.dataSize / sizeof(T), this->file);
    if (actual != currentSubrecordHead.dataSize) {
        std::stringstream s;
        s << "Expected to read size " << currentSubrecordHead.dataSize << ", actually read " << actual << " bytes,\n";
        s << " in subrecord " << Util::typeValueToName(currentSubrecordHead.type) << ", in record " << Util::typeValueToName(currentRecordHead.type) << " at " << std::hex << std::ftell(this->file);
        log_fatal(s.str().c_str());
        throw std::runtime_error("Read mismatch!");
    }
}

template <typename T>
void ESMReader::readFixedArraySubrecord(T* array)
{
    int actual = std::fread(array, sizeof(T), currentSubrecordHead.dataSize / sizeof(T), this->file);
    if (actual != currentSubrecordHead.dataSize) {
        std::stringstream s;
        s << "Expected to read size " << currentSubrecordHead.dataSize << ", actually read " << actual << " bytes,\n";
        s << " in subrecord " << Util::typeValueToName(currentSubrecordHead.type) << ", in record " << Util::typeValueToName(currentRecordHead.type) << " at " << std::hex << std::ftell(this->file);
        log_fatal(s.str().c_str());
        throw std::runtime_error("Read mismatch!");
    }
}

}
