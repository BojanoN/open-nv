#pragma once

#include <cstdio>
#include <string>

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

    RecordHeader&    getCurrentRecord();
    SubrecordHeader& getCurrentSubrecord();
    ESMName          getCurrentRecordType();
    ESMName          subrecordType();

    uint16_t subrecordSize() { return currentSubrecordHead.dataSize; };

    void readNextSubrecordHeader();
    void readNextRecordHeader();
    void readNextGroupHeader();
    void readNextHeader();

    void checkSubrecordHeader(ESMType type);

    ESMType peekNextType();

    template <typename T>
    void readGeneric(T* dest);

    std::string getFileName() { return this->fileName; };

    template <typename T>
    void readSubrecord(T& subrecValue);

    template <typename T>
    void readArraySubrecord(T* array);

    template <typename T>
    void readRawData(T& value);

    template <typename T>
    int readRawArray(T* array, ssize_t length);

    /*
    std::string readCstring();
    int32_t readInt32();
    //uint32_t readUInt32();
    uint16_t readUInt16();
    float readFloat32()
    */

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
}
