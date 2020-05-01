#include "reader.hpp"
#include "logc/log.h"
#include "util/compress.hpp"

namespace ESM {

RecordHeader&    ESMReader::getCurrentRecord() { return currentRecordHead; };
SubrecordHeader& ESMReader::getCurrentSubrecord() { return currentSubrecordHead; };
GroupHeader&     ESMReader::getCurrentGroup() { return currentGroupHead; };

uint32_t    ESMReader::recordType() { return this->currentRecordHead.type; }
ESMType     ESMReader::subrecordType() { return this->currentSubrecordHead.type; }
uint32_t    ESMReader::groupLabel() { return currentGroupHead.label; }
GroupType   ESMReader::groupType() { return currentGroupHead.groupType; }
RecordFlags ESMReader::recordFlags() { return currentRecordHead.flags; }
uint32_t    ESMReader::recordId() { return currentRecordHead.id; }

ESMType ESMReader::peekNextType()
{
    ESMType ret;
    this->currentStream->read(reinterpret_cast<char*>(&ret), sizeof(uint32_t));
    this->currentStream->seekg(-sizeof(uint32_t), std::ios::cur);
    return ret;
}

void ESMReader::readNextRecordHeader()
{
    this->currentStream->read(reinterpret_cast<char*>(&(this->currentRecordHead)), sizeof(RecordHeader));
    //this->endOfRecord = (int)this->currentStream->tellg() + currentRecordHead.dataSize;
    updateReadLocation(sizeof(RecordHeader));
    updateEndOfRecord();
}

void ESMReader::readNextGroupHeader()
{
    this->currentStream->read(reinterpret_cast<char*>(&(this->currentGroupHead)), sizeof(GroupHeader));
    //this->endOfGroup = (int)this->currentStream->tellg() + currentGroupHead.groupSize - 24;
    updateReadLocation(sizeof(GroupHeader));
    updateEndOfGroup();
}

void ESMReader::readNextSubrecordHeader()
{
    /* if (this->currentStream->tellg() == endOfRecord) {
        std::stringstream s;
        s << "Unexpected end of record " << currentRecordHead.type << " at "
          << this->currentStream->tellg() << '\n';
    }*/

    // Check if we ran out of compressed data
    /*if (!currentStream && this->file) {
        this->currentStream   = &this->file;
        this->currentLocation = this->currentLocationBackup;
        std::streambuf* stb   = this->compressed.rdbuf();
        delete stb;
    }*/
    this->currentStream->read(reinterpret_cast<char*>(&(this->currentSubrecordHead)), sizeof(SubrecordHeader));
    updateReadLocation(sizeof(SubrecordHeader));
    updateEndOfSubrecord();
    //this->endOfSubrecord = (int)this->currentStream->tellg() + currentSubrecordHead.dataSize;
}

void ESMReader::skipRecord()
{
    endCompressedMode();
    this->currentStream->seekg(endOfRecord, std::ios::beg);
    updateReadLocation(endOfRecord - currentLocation);
}

void ESMReader::skipGroup()
{
    endCompressedMode();
    this->currentStream->seekg(endOfGroup, std::ios::beg);
    updateReadLocation(endOfGroup - currentLocation);
}

void ESMReader::skipSubrecord()
{
    endCompressedMode();
    this->currentStream->seekg(endOfSubrecord, std::ios::beg);
    updateReadLocation(endOfSubrecord - currentLocation);
}

void ESMReader::checkSubrecordHeader(ESMType type)
{
    if (currentSubrecordHead.type != type) {
        log_fatal("Expected subrecord type %s got %s at record type %s, formid %u at 0x%06x",
            Util::typeValueToName(type).c_str(),
            Util::typeValueToName(currentSubrecordHead.type).c_str(),
            Util::typeValueToName(currentRecordHead.type).c_str(),
            currentRecordHead.id,
            ((int)this->currentStream->tellg() - sizeof(SubrecordHeader)));

        throw std::runtime_error("Subrecord type mismatch!");
    }
}

void ESMReader::readStringSubrecord(std::string& subrecString)
{
    subrecString.resize(currentSubrecordHead.dataSize);

    //int start = this->currentStream->tellg();
    this->currentStream->read(reinterpret_cast<char*>(&subrecString[0]), currentSubrecordHead.dataSize);

    if (!currentStream) {
        std::stringstream s;
        s << "I/O error!\n";
        s << "Expected to read array with size " << currentSubrecordHead.dataSize << "\n";
        s << " in subrecord " << Util::typeValueToName(currentSubrecordHead.type) << ", in record " << Util::typeValueToName(currentRecordHead.type) << " at " << std::hex << this->currentStream->tellg();
        log_fatal(s.str().c_str());
        throw std::runtime_error("Read mismatch!");
    }
    updateReadLocation(currentSubrecordHead.dataSize);
}

void ESMReader::rewind(ssize_t size)
{
    this->currentStream->seekg(-size, std::ios::cur);
    updateReadLocation(-size);
}

void ESMReader::readFixedSizeString(std::string& dest, size_t size)
{
    dest.resize(size);

    this->currentStream->read(reinterpret_cast<char*>(&dest[0]), size);

    if (!currentStream) {
        std::stringstream s;
        s << "I/O error!\n";
        s << "Expected to read array with size " << currentSubrecordHead.dataSize << "\n";
        s << " in subrecord " << Util::typeValueToName(currentSubrecordHead.type) << ", in record " << Util::typeValueToName(currentRecordHead.type) << " at " << std::hex << this->currentStream->tellg();
        log_fatal(s.str().c_str());
        throw std::runtime_error("Read mismatch!");
    }
    updateReadLocation(size);
}

uint32_t ESMReader::getCurrentPosition()
{
    if (this->currentStream) {
        return currentLocation;
    } else {
        throw std::runtime_error("File has not been opened yet!");
    }
}
void ESMReader::reportError(std::string err)
{
    log_fatal("Error encountered at 0x%06x", currentLocation);
    throw std::runtime_error(err);
}

void ESMReader::startCompressedMode()
{
    uint32_t recordSize;
    this->currentStream->read(reinterpret_cast<char*>(&recordSize), sizeof(uint32_t));

    this->currentLocation += this->currentRecordHead.dataSize;

    this->compBuf = new decompBuf(this->file, this->currentRecordHead.dataSize, recordSize);

    this->compressed.rdbuf(this->compBuf);
    this->currentStream = &this->compressed;
    savedContext.save(*this);
    this->endOfRecord     = recordSize;
    this->currentLocation = 0;
}

void ESMReader::endCompressedMode()
{
    if (currentRecordHead.flags & RecordFlags::COMPRESSED) {
        savedContext.restore(*this);
        currentStream = &file;
        this->currentStream->seekg(this->currentLocation, std::ios::beg);

        if (compBuf != nullptr) {
            delete compBuf;
            compBuf = nullptr;
        }
    }
}

void ESMReader::ReaderContext::save(ESMReader& reader)
{
    currentLocation = reader.currentLocation;
    endOfSubrecord  = reader.endOfSubrecord;
    endOfRecord     = reader.endOfRecord;
    endOfGroup      = reader.endOfGroup;
}

void ESMReader::ReaderContext::restore(ESMReader& reader)
{
    reader.currentLocation = currentLocation;
    reader.endOfSubrecord  = endOfSubrecord;
    reader.endOfRecord     = endOfRecord;
    reader.endOfGroup      = endOfGroup;
}

void ESMReader::readStringArray(std::vector<std::string>& vec)
{
    std::vector<uint8_t> tmp(currentSubrecordHead.dataSize);
    this->currentStream->read(reinterpret_cast<char*>(&tmp[0]), currentSubrecordHead.dataSize);
    updateReadLocation(currentSubrecordHead.dataSize);

    uint32_t beg = 0;

    for (uint32_t i = 0; i < currentSubrecordHead.dataSize; i++) {
        if (tmp[i] == 0) {
            vec.emplace_back(reinterpret_cast<char*>(&tmp[beg]), i - beg);
            beg = i + 1;
        }
    }
}

};
