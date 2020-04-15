#include "reader.hpp"
#include "records.hpp"
#include <cassert>
using namespace ESM;

RecordHeader&    ESMReader::getCurrentRecord() { return currentRecordHead; };
SubrecordHeader& ESMReader::getCurrentSubrecord() { return currentSubrecordHead; };

uint32_t ESMReader::recordType() { return this->currentRecordHead.type; };
uint32_t ESMReader::subrecordType() { return this->currentSubrecordHead.type; };

uint32_t ESMReader::peekNextType()
{
    if (std::ftell(this->file) != endOfSubrecord) {
        std::stringstream s;
        s << "Cannot peek when not at end of subrecord.\n";
        s << currentRecordHead.type << " at " << std::ftell(this->file) << '\n';
    }
    uint32_t ret;

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
    this->endOfSubrecord = std::ftell(this->file) + currentSubrecordHead.dataSize;
}

void ESMReader::skipRecord()
{
    std::fseek(this->file, endOfRecord, SEEK_SET);
}

void ESMReader::skipGroup()
{
    std::fseek(this->file, endOfGroup, SEEK_SET);
}

void ESMReader::checkSubrecordHeader(ESMType type)
{
    if (currentSubrecordHead.type != type) {
        std::stringstream s;
        /*s << "Expected subrecord type " << type[0]
          << type[1] << type[2] << type[3];*/
        s << " at record type " << currentRecordHead.type;
        s << ", id " << currentRecordHead.id << "at " << std::ftell(this->file) << '\n';
        throw std::runtime_error(s.str());
    }
}
