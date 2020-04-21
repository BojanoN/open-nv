#include "reader.hpp"
#include "logc/log.h"
#include "util/compress.hpp"

namespace ESM {

RecordHeader&    ESMReader::getCurrentRecord() { return currentRecordHead; };
SubrecordHeader& ESMReader::getCurrentSubrecord()
{
    return currentSubrecordHead;
};

uint32_t    ESMReader::recordType() { return this->currentRecordHead.type; }
uint32_t    ESMReader::subrecordType() { return this->currentSubrecordHead.type; }
uint32_t    ESMReader::groupLabel() { return currentGroupHead.label; }
int32_t     ESMReader::groupType() { return currentGroupHead.groupType; }
RecordFlags ESMReader::recordFlags() { return currentRecordHead.flags; }
uint32_t    ESMReader::recordId() { return currentRecordHead.id; }

uint32_t ESMReader::peekNextType()
{
    uint32_t ret;

    if (this->currentStream.tellg() != endOfSubrecord) {

        log_warn("Cannot peek when not at end of subrecord");
        log_warn("At 0x%06x", (int)this->currentStream.tellg());
        log_warn("%.4s", currentRecordHead.type);
    }

    this->currentStream.read(reinterpret_cast<char*>(&ret), sizeof(uint32_t));
    this->currentStream.seekg(-sizeof(uint32_t), std::ios::cur);
    return ret;
}

void ESMReader::readNextRecordHeader()
{
    this->currentStream.read(reinterpret_cast<char*>(&(this->currentRecordHead)), sizeof(RecordHeader));
    this->endOfRecord = (int)this->currentStream.tellg() + currentRecordHead.dataSize;
}

void ESMReader::readNextGroupHeader()
{
    this->currentStream.read(reinterpret_cast<char*>(&(this->currentGroupHead)), sizeof(GroupHeader));
    this->endOfGroup = (int)this->currentStream.tellg() + currentGroupHead.groupSize - 24;
}

void ESMReader::readNextSubrecordHeader()
{
    if (this->currentStream.tellg() == endOfRecord) {
        std::stringstream s;
        s << "Unexpected end of record " << currentRecordHead.type << " at "
          << this->currentStream.tellg() << '\n';
    }
    this->currentStream.read(reinterpret_cast<char*>(&(this->currentSubrecordHead)), sizeof(SubrecordHeader));
    this->endOfSubrecord = (int)this->currentStream.tellg() + currentSubrecordHead.dataSize;
}

void ESMReader::skipRecord() { this->currentStream.seekg(endOfRecord, std::ios::beg); }

void ESMReader::skipGroup() { this->currentStream.seekg(endOfGroup, std::ios::beg); }

void ESMReader::skipSubrecord() { this->currentStream.seekg(endOfSubrecord, std::ios::beg); }

void ESMReader::checkSubrecordHeader(ESMType type)
{
    if (currentSubrecordHead.type != type) {
        log_fatal("Expected subrecord type %s got %s at record type %s, formid %u at 0x%06x",
            Util::typeValueToName(type).c_str(),
            Util::typeValueToName(currentSubrecordHead.type).c_str(),
            Util::typeValueToName(currentRecordHead.type).c_str(),
            currentRecordHead.id,
            ((int)this->currentStream.tellg() - sizeof(SubrecordHeader)));

        throw std::runtime_error("Subrecord type mismatch!");
    }
}

void ESMReader::readStringSubrecord(std::string& subrecString)
{
    subrecString.resize(currentSubrecordHead.dataSize);

    int start = this->currentStream.tellg();
    this->currentStream.read(reinterpret_cast<char*>(&subrecString[0]), currentSubrecordHead.dataSize);

    int end    = this->currentStream.tellg();
    int actual = end - start;

    if (actual != currentSubrecordHead.dataSize) {
        log_fatal("Expected to read size %u, actually read %u bytes", currentSubrecordHead.dataSize, actual);
        log_fatal("In subrecord %s", Util::typeValueToName(this->currentSubrecordHead.type).c_str());
        log_fatal("At record type %s, formid %u at 0x%06x",
            Util::typeValueToName(this->currentRecordHead.type).c_str(),
            this->currentRecordHead.id,
            this->currentStream.tellg());

        throw std::runtime_error("Read mismatch!");
    }
}

void ESMReader::rewind(ssize_t size)
{
    this->currentStream.seekg(-size, std::ios::cur);
}

void ESMReader::readFixedSizeString(std::string& dest, size_t size)
{
    dest.resize(size);
    int start = this->currentStream.tellg();

    this->currentStream.read(reinterpret_cast<char*>(&dest[0]), size);

    int end    = this->currentStream.tellg();
    int actual = end - start;

    if (actual != size) {
        log_fatal("Expected to read size %u, actually read %u bytes", size, actual);
        log_fatal("In subrecord %s", Util::typeValueToName(this->currentSubrecordHead.type).c_str());
        log_fatal("At record type %s, formid %u at 0x%06x",
            Util::typeValueToName(this->currentRecordHead.type).c_str(),
            this->currentRecordHead.id,
            this->currentStream.tellg());

        throw std::runtime_error("Read mismatch!");
    }
}

uint32_t ESMReader::getCurrentPosition()
{
    if (this->currentStream) {
        return this->currentStream.tellg();
    } else {
        throw std::runtime_error("File has not been opened yet!");
    }
}
void ESMReader::reportError(std::string err)
{
    //std::stringstream s(err, std::ios_base::out | std::ios_base::app);
    //std::cerr << err;
    //  s << " in record type " << Util::typeValueToName(currentRecordHead.type) << " at 0x" << std::hex << this->currentStream.tellg();
    log_fatal("Error encountered at 0x%06x", this->currentStream.tellg());
    throw std::runtime_error(err);
}

void ESMReader::readCompressed()
{
    CompressedDataHeader hdr;
    this->currentStream.read(reinterpret_cast<char*>(&hdr), sizeof(CompressedDataHeader));

    std::vector<uint8_t> decomp(hdr.decompSize);
    std::vector<uint8_t> comp(this->currentRecordHead.dataSize);

    this->currentStream.read(reinterpret_cast<char*>(&comp[0]), this->currentRecordHead.dataSize);
    ::Util::zlibDecompress(comp, decomp);
}

};
