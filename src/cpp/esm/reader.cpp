#include "reader.hpp"
#include "logc/log.h"

namespace ESM {

RecordHeader&    ESMReader::getCurrentRecord() { return currentRecordHead; };
SubrecordHeader& ESMReader::getCurrentSubrecord()
{
    return currentSubrecordHead;
};

uint32_t ESMReader::recordType() { return this->currentRecordHead.type; }
uint32_t ESMReader::subrecordType() { return this->currentSubrecordHead.type; }
uint32_t ESMReader::groupLabel() { return currentGroupHead.label; }
int32_t  ESMReader::groupType() { return currentGroupHead.groupType; }
uint32_t ESMReader::recordFlags() { return currentRecordHead.flags; }
uint32_t ESMReader::recordId() { return currentRecordHead.id; }

uint32_t ESMReader::peekNextType()
{
    uint32_t ret;

    if (std::ftell(this->file) != endOfSubrecord) {

        log_warn("Cannot peek when not at end of subrecord");
        log_warn("%.4s at 0x%06x", currentRecordHead.type, std::ftell(this->file));
    }

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
        s << "Unexpected end of record " << currentRecordHead.type << " at "
          << std::ftell(this->file) << '\n';
    }
    std::fread(&(this->currentSubrecordHead), sizeof(SubrecordHeader), 1,
        this->file);
    this->endOfSubrecord = std::ftell(this->file) + currentSubrecordHead.dataSize;
}

void ESMReader::skipRecord() { std::fseek(this->file, endOfRecord, SEEK_SET); }

void ESMReader::skipGroup() { std::fseek(this->file, endOfGroup, SEEK_SET); }

void ESMReader::skipSubrecord() { std::fseek(this->file, endOfSubrecord, SEEK_SET); }

void ESMReader::checkSubrecordHeader(ESMType type)
{
    if (currentSubrecordHead.type != type) {
        log_fatal("Expected subrecord type %s at record type %s, formid %u at 0x%06x",
            Util::typeValueToName(type).c_str(),
            Util::typeValueToName(currentRecordHead.type).c_str(),
            currentRecordHead.id,
            (std::ftell(this->file) - sizeof(SubrecordHeader)));

        throw std::runtime_error("Subrecord type mismatch!");
    }
}

void ESMReader::readStringSubrecord(std::string& subrecString)
{
    subrecString.resize(currentSubrecordHead.dataSize);
    int actual = std::fread(&subrecString[0], sizeof(char),
        currentSubrecordHead.dataSize / sizeof(char), this->file);
    if (actual != currentSubrecordHead.dataSize) {
        log_fatal("Expected to read size %u, actually read %u bytes", currentSubrecordHead.dataSize, actual);
        log_fatal("In subrecord %s", Util::typeValueToName(this->currentSubrecordHead.type).c_str());
        log_fatal("At record type %s, formid %u at 0x%06x",
            Util::typeValueToName(this->currentRecordHead.type).c_str(),
            this->currentRecordHead.id,
            std::ftell(this->file));

        throw std::runtime_error("Read mismatch!");
    }
}

void ESMReader::rewind(ssize_t size)
{
    std::fseek(this->file, -size, SEEK_CUR);
}

void ESMReader::readFixedSizeString(std::string& dest, size_t size)
{
    dest.resize(size);
    int actual = std::fread(&dest[0], sizeof(char),
        size, this->file);
    if (actual != size) {
        log_fatal("Expected to read size %u, actually read %u bytes", size, actual);
        log_fatal("In subrecord %s", Util::typeValueToName(this->currentSubrecordHead.type).c_str());
        log_fatal("At record type %s, formid %u at 0x%06x",
            Util::typeValueToName(this->currentRecordHead.type).c_str(),
            this->currentRecordHead.id,
            std::ftell(this->file));

        throw std::runtime_error("Read mismatch!");
    }
}

uint32_t ESMReader::getCurrentPosition()
{
    if (this->file) {
        return std::ftell(this->file);
    } else {
        throw std::runtime_error("File has not been opened yet!");
    }
}
void ESMReader::reportError(std::string err)
{
    //std::stringstream s(err, std::ios_base::out | std::ios_base::app);
    //std::cerr << err;
    //  s << " in record type " << Util::typeValueToName(currentRecordHead.type) << " at 0x" << std::hex << std::ftell(this->file);
    log_fatal("Error encountered at 0x%06x", std::ftell(this->file));
    throw std::runtime_error(err);
}

};
