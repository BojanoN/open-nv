#include "reader.hpp"
#include "records.hpp"
#include <cassert>
using namespace ESM;

RecordHeader&    ESMReader::getCurrentRecord() { return currentRecordHead; };
SubrecordHeader& ESMReader::getCurrentSubrecord() { return currentSubrecordHead; };

uint32_t ESMReader::getCurrentRecordType() { return this->currentRecordHead.Type; };
uint32_t ESMReader::getCurrentSubrecordType() { return this->currentSubrecordHead.Type; };

ESMType ESMReader::peekNextType()
{
    if (std::ftell(this->file) != endOfSubrecord) {
        std::stringstream s;
        s << "Cannot peek when not at end of subrecord.\n";
        s << currentRecordHead.type << " at " << std::ftell(this->file) << '\n';
    }
    ESMType ret;

    std::fread(&ret, sizeof(uint32_t), 1, this->file);
    std::fseek(this->file, -sizeof(uint32_t), SEEK_CUR);

    return ret;
}

void ESMReader::readNextRecordHeader()
{
    std::fread(&(this->currentRecordHead), sizeof(RecordHeader), 1, this->file);
    this->endOfRecord = std::ftell(this->file) + currentRecordHead.dataSize;
}

void ESMReader::readNextGroupHeader()
{
    std::fread(&(this->currentGroupHead), sizeof(GroupHeader), 1, this->file);
    this->endOfGroup = std::ftell(this->file) + currentGroupHead.groupSize - 24;
}

void ESMReader::readNextSubrecordHeader()
{
    if (std::ftell(this->file) == endOfRecord) {
        std::stringstream s;
        s << "Unexpected end of record " << currentRecordHead.type << " at " << std::ftell(this->file) << '\n';
    }
    std::fread(&(this->currentSubrecordHead), sizeof(SubrecordHeader), 1, this->file);
    this->endOfSubecord = std::ftell(this->file) + currentSubrecordHead.dataSize;
}

void ESMReader::skipRecord()
{
    std::fseek(this->file, endOfRecord, SEEK_SET);
}

void ESMReader::skipGroup()
{
    std::fseek(this->file, endOfGroup, SEEK_SET);
}

/*
std::string ESMReader::readCstring() {
    std::string s;
    int actual = std::fread(&s, sizeof(char), currentSubrecordHead.dataSize, this->file);
    if (actual != currentSubrecordHead.dataSize) {
        throw std::runtime_error();
    }
    return s;
}*/

template <typename T>
void ESMReader::readSubrecord(T& subrecValue)
{
    int actual = std::fread(&subrecValue, sizeof(T), 1, this->file);
    if (actual != currentSubrecordHead.dataSize) {
        std::stringstream s;
        s << "Expected to read size " << currentSubrecordHead.dataSize << ", actually read " << acual << " bytes,\n";
        s << " in subrecord " << currentSubrecordHead.type << ", in record " << currentRecordHead.type << " at " << std::ftell(this - file) << '\n';
        throw std::runtime_error(s.str());
    }
}

template <typename T>
int ESMReader::readRawData(T& value)
{
    return std::fread(&value, sizeof(T), 1, this->file);
}

template <typename T>
void readArraySubrecord(T* subrecPtr)
{
    int actual = std::fread(subrecPtr, sizeof(T), currentSubrecordHead.dataSize / sizeof(T), this->file);
    if (actual != currentSubrecordHead.dataSize) {
        std::stringstream s;
        s << "Expected to read size " << currentSubrecordHead.dataSize << ", actually read " << acual << " bytes,\n";
        s << " in subrecord " << currentSubrecordHead.type << ", in record " << currentRecordHead.type << " at " << std::ftell(this - file) << '\n';
        throw std::runtime_error(s.str());
    }
}

template <typename T>
int readRawArray(T* array, ssize_t length)
{
    return std::fread(array, sizeof(T), length, this->file);
}

/*
int32_t ESMReader::readInt32() {
    int32_t i;
    int actual = std::fread(&i, sizeof(int32_t), 1, this->file);
    if(actual != 1) {
       throw std::runtime_error; 
    }
    return i;
}

uint16_t ESMReader::readInt32() {
    int32_t i;
    int actual = std::fread(&i, sizeof(int32_t), 1, this->file);
    if(actual != 1) {
       throw std::runtime_error; 
    }
    return i;
}


float ESMReader::readFloat32() {
    float f;
    int actual = std::fread(&f, sizeof(float), 1, this->file);
    if(actual != 1) {
       throw std::runtime_error; 
    }
    return f;
}*/

void ESMReader::checkSubrecordHeader(ESMType type)
{
    if (currentSubrecordHead.type.intValue != type.intValue) {
        std::stringstream s;
        s << "Expected subrecord type " << type[0]
          << type[1] << type[2] << type[3];
        s << " at record type " << currentRecordHead.type;
        s << ", id " << currentRecordHead.id << "at " << std::ftell(this->file) << '\n';
        throw std::runtime_error(s.str());
    }
}
