#include "reader.hpp"
#include "records.hpp"
#include <cassert>
using namespace ESM;

RecordHeader&    ESMReader::getCurrentRecord() { return currentRecordHead; };
SubrecordHeader& ESMReader::getCurrentSubrecord() { return currentSubrecordHead; };

uint32_t ESMReader::getCurrentRecordType() { return this->currentRecordHead.Type; };
uint32_t ESMReader::getCurrentSubrecordType() { return this->currentSubrecordHead.Type; };

uint32_t ESMReader::peekNextType()
{
    uint32_t ret;

    std::fread(&ret, sizeof(uint32_t), 1, this->file);
    std::fseek(this->file, -sizeof(uint32_t), SEEK_CUR);

    return ret;
}

void ESMReader::readNextRecord()
{
    std::fread(&(this->currentRecordHead), sizeof(RecordHeader), 1, this->file);
    this->fileLeft -= sizeof(RecordHeader);
    this->recordLeft = currentRecordHead.DataSize;
}

void ESMReader::readNextSubrecord()
{
    std::fread(&(this->currentSubrecordHead), sizeof(SubrecordHeader), 1, this->file);
    this->fileLeft -= sizeof(SubrecordHeader);
    this->recordLeft -= sizeof(SubrecordHeader);
#ifdef DEBUG
    assert(this->recordLeft > 0);
#endif
}
template <typename T>
void ESMReader::readGeneric(T* dest)
{
    std::fread(dest, sizeof(T), 1, this->file);
    this->fileLeft -= sizeof(T);
    this->recordLeft -= sizeof(T);
#ifdef DEBUG
    assert(this->recordLeft > 0);
#endif
}
