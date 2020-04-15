#pragma once

#include <cstdio>
#include <string>
#include <sstream>
#include <ostream>

#include "record.hpp"
#include "subrecord.hpp"

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
    ~ESMReader() {};

    bool hasMoreSubrecords() { return std::ftell(this->file) < endOfRecord; };
    bool hasMoreRecordsInGroup() { return std::ftell(this->file) < endOfGroup; }
    bool hasMoreBytes() { return std::ftell(this->file) < fileSize; };
    void skipRecord();
    void skipGroup();

    RecordHeader&    getCurrentRecord();
    SubrecordHeader& getCurrentSubrecord();
    uint32_t          recordType();
    uint32_t          subrecordType();

    uint16_t subrecordSize() { return currentSubrecordHead.dataSize; };

    void readNextSubrecordHeader();
    void readNextRecordHeader();
    void readNextGroupHeader();
    void readNextHeader();

    void checkSubrecordHeader(ESMType type);

    uint32_t peekNextType();

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
    int actual = std::fread(&subrecValue, sizeof(T), 1, this->file);
    if (actual != currentSubrecordHead.dataSize) {
        std::stringstream s;
        s << "Expected to read size " << currentSubrecordHead.dataSize << ", actually read " << actual << " bytes,\n";
        s << " in subrecord " << currentSubrecordHead.type << ", in record " << currentRecordHead.type << " at " << std::ftell(this->file) << '\n';
        throw std::runtime_error(s.str());
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
        s << " in subrecord " << currentSubrecordHead.type << ", in record " << currentRecordHead.type << " at " << std::ftell(this->file) << '\n';
        throw std::runtime_error(s.str());
    }
}

template <typename T>
void ESMReader::readFixedArraySubrecord(T* array) {
    int actual = std::fread(array, sizeof(T), currentSubrecordHead.dataSize / sizeof(T), this->file);
    if (actual != currentSubrecordHead.dataSize) {
        std::stringstream s;
        s << "Expected to read size " << currentSubrecordHead.dataSize << ", actually read " << actual << " bytes,\n";
        s << " in subrecord " << currentSubrecordHead.type << ", in record " << currentRecordHead.type << " at " << std::ftell(this->file) << '\n';
        throw std::runtime_error(s.str());
    }
}


void ESMReader::readStringSubrecord(std::string& subrecString) {
    subrecString.resize(currentSubrecordHead.dataSize);
    int actual = std::fread(&subrecString[0], sizeof(char), currentSubrecordHead.dataSize / sizeof(char), this->file);
    if (actual != currentSubrecordHead.dataSize) {
        std::stringstream s;
        s << "Expected to read size " << currentSubrecordHead.dataSize << ", actually read " << actual << " bytes,\n";
        s << " in subrecord " << currentSubrecordHead.type << ", in record " << currentRecordHead.type << " at " << std::ftell(this->file) << '\n';
        throw std::runtime_error(s.str());
    }
}

}
