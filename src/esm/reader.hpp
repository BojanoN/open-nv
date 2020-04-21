#pragma once

#include "headers.hpp"
#include "logc/log.h"
#include "types.hpp"
#include "util/bytestream.hpp"
#include "utils.hpp"

#include <cassert>
//#include <cerrno>
//#include <cstdio>
//#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace ESM {

class ESMReader {
public:
    ESMReader(std::string& path)
        : currentStream { &file }
        , file { path.c_str() }
        , compressed(nullptr)
        , fileName { path }
        , currentLocation(0)

    {
        this->file.seekg(0, std::ios::end);

        ssize_t size   = this->file.tellg();
        this->fileSize = size;
        this->fileLeft = size;

        this->file.seekg(0, std::ios::beg);
    };
    ~ESMReader() {};

    bool hasMoreSubrecords() { return this->file.tellg() < endOfRecord; }
    bool hasMoreRecordsInGroup() { return this->file.tellg() < endOfGroup; }
    bool hasMoreBytes() { return this->file.tellg() < fileSize; }
    void skipRecord();
    void skipGroup();
    void skipSubrecord();
    void setCurrentStream(std::ifstream* stream)
    {
        this->currentStream = stream;
    }

    RecordHeader&    getCurrentRecord();
    SubrecordHeader& getCurrentSubrecord();
    uint32_t         recordType();
    uint32_t         subrecordType();
    uint32_t         groupLabel();
    int32_t          groupType();

    RecordFlags recordFlags();
    formid      recordId();

    uint16_t subrecordSize() { return currentSubrecordHead.dataSize; }

    void readNextSubrecordHeader();
    void readNextRecordHeader();
    void readNextGroupHeader();
    void readNextHeader();
    void readCompressed();

    inline void updateReadLocation(ssize_t readSize)
    {
        this->currentLocation += readSize;
    }
    inline void updateEndOfRecord()
    {
        this->endOfRecord = currentLocation + currentRecordHead.dataSize;
    }
    inline void updateEndOfGroup()
    {
        this->endOfGroup = currentLocation + currentGroupHead.groupSize - 24;
    }
    inline void updateEndOfSubrecord()
    {
        this->endOfSubrecord = currentLocation + currentSubrecordHead.dataSize;
    }

    void checkSubrecordHeader(ESMType type);

    uint32_t peekNextType();
    void     rewind(ssize_t size);
    uint32_t getCurrentPosition();

    std::string getFileName() { return this->fileName; };

    template <typename T>
    void readSubrecord(T& subrecValue);

    void readStringSubrecord(std::string& subrecString);

    template <typename T>
    void readArraySubrecord(std::vector<T>& array);

    template <typename T>
    void readRawData(T& value);

    template <typename T>
    void readRawArray(T* array, ssize_t length);

    template <typename T>
    void readFixedArraySubrecord(T* array);

    void readFixedSizeString(std::string& dest, size_t size);

    void
    reportError(std::string err);

private:
    std::istream* currentStream;
    std::ifstream file;
    std::istream  compressed;

    ssize_t     fileSize;
    std::string fileName;

    ssize_t fileLeft;
    ssize_t recordLeft;
    ssize_t groupLeft;

    ssize_t endOfRecord;
    ssize_t endOfGroup;
    ssize_t endOfSubrecord;
    ssize_t currentLocation;
    ssize_t currentLocationBackup;

    GroupHeader     currentGroupHead;
    RecordHeader    currentRecordHead;
    SubrecordHeader currentSubrecordHead;
};

template <typename T>
void ESMReader::readSubrecord(T& subrecValue)
{

    //int start = this->currentStream->tellg();
    this->currentStream->read(reinterpret_cast<char*>(&subrecValue), sizeof(T));

    if (!currentStream) {
        std::stringstream s;
        s << "I/O error!\n";
        s << "Expected to read record with size " << currentSubrecordHead.dataSize << "\n";
        s << " in subrecord " << Util::typeValueToName(currentSubrecordHead.type) << ", in record " << Util::typeValueToName(currentRecordHead.type) << " at " << std::hex << this->currentStream->tellg();
        log_fatal(s.str().c_str());
        throw std::runtime_error("Read mismatch!");
    }
    updateReadLocation(sizeof(T));

    //int end    = this->currentStream->tellg();
    /*int actual = end - start;
    if (actual != sizeof(T)) {
        std::stringstream s;
        s << "Expected to read " << sizeof(T) << " bytes, actually read " << actual << " bytes,\n";
        s << " in subrecord " << Util::typeValueToName(currentSubrecordHead.type) << ", in record " << Util::typeValueToName(currentRecordHead.type) << " at 0x" << std::hex << this->currentStream->tellg();
        log_fatal(s.str().c_str());
        throw std::runtime_error("Read mismatch!");
    }*/
}

template <typename T>
void ESMReader::readRawData(T& value)
{
    //int start = this->currentStream->tellg();
    this->currentStream->read(reinterpret_cast<char*>(&value), sizeof(T));
    //int end = this->currentStream->tellg();
    updateReadLocation(sizeof(T));

    //return end - start;
}

template <typename T>
void ESMReader::readRawArray(T* array, ssize_t length)
{

    //int start = this->currentStream->tellg();
    this->currentStream->read(reinterpret_cast<char*>(array), sizeof(T) * length);
    //int end = this->currentStream->tellg();
    updateReadLocation(length * sizeof(T));

    //return end - start;
}

template <typename T>
void ESMReader::readArraySubrecord(std::vector<T>& array)
{
    array.resize(currentSubrecordHead.dataSize / sizeof(T));
    //int start = this->currentStream->tellg();
    this->currentStream->read(reinterpret_cast<char*>(&array[0]), currentSubrecordHead.dataSize);

    if (!currentStream) {
        std::stringstream s;
        s << "I/O error!\n";
        s << "Expected to read array with size " << currentSubrecordHead.dataSize << "\n";
        s << " in subrecord " << Util::typeValueToName(currentSubrecordHead.type) << ", in record " << Util::typeValueToName(currentRecordHead.type) << " at " << std::hex << this->currentStream->tellg();
        log_fatal(s.str().c_str());
        throw std::runtime_error("Read mismatch!");
    }
    updateReadLocation(currentSubrecordHead.dataSize);
    //Nikad nece biti error, rijec je o citanju "koliko treba"

    //int end    = this->currentStream->tellg();
    /*int actual = end - start;

    if (actual != currentSubrecordHead.dataSize) {
        std::stringstream s;
        s << "Expected to read size " << currentSubrecordHead.dataSize << ", actually read " << actual << " bytes,\n";
        s << " in subrecord " << Util::typeValueToName(currentSubrecordHead.type) << ", in record " << Util::typeValueToName(currentRecordHead.type) << " at " << std::hex << this->currentStream->tellg();
        log_fatal(s.str().c_str());
        throw std::runtime_error("Read mismatch!");
    }*/
}

template <typename T>
void ESMReader::readFixedArraySubrecord(T* array)
{
    //int start = this->currentStream->tellg();
    this->currentStream->read(reinterpret_cast<char*>(array), currentSubrecordHead.dataSize);

    if (!currentStream) {
        std::stringstream s;
        s << "I/O error!\n";
        s << "Expected to read array with size " << currentSubrecordHead.dataSize << "\n";
        s << " in subrecord " << Util::typeValueToName(currentSubrecordHead.type) << ", in record " << Util::typeValueToName(currentRecordHead.type) << " at " << std::hex << this->currentStream->tellg();
        log_fatal(s.str().c_str());
        throw std::runtime_error("Read mismatch!");
    }
    //int end    = this->currentStream->tellg();
    /*int actual = end - start;
    if (actual != currentSubrecordHead.dataSize) {
        std::stringstream s;
        s << "Expected to read size " << currentSubrecordHead.dataSize << ", actually read " << actual << " bytes,\n";
        s << " in subrecord " << Util::typeValueToName(currentSubrecordHead.type) << ", in record " << Util::typeValueToName(currentRecordHead.type) << " at " << std::hex << this->currentStream->tellg();
        log_fatal(s.str().c_str());
        throw std::runtime_error("Read mismatch!");
    }*/
    updateReadLocation(currentSubrecordHead.dataSize);
}

}
