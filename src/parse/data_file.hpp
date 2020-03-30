#pragma once

#include <cstdint>
#include <fstream>
#include <unordered_map>
#include <vector>

#include "../util/octet_hash.hpp"

class SubRecord {
public:
    uint8_t Type[4];
    uint16_t DataSize;
    uint8_t* Data; // u_int8_t Data[DataSize] for now, later a vector

    SubRecord(std::ifstream& stream)
    {
    }
};

class Record {
public:
    uint8_t Type[4];
    uint32_t DataSize;
    uint32_t Flags;
    uint32_t ID;
    uint32_t CreationKitRevision;
    uint16_t FormVersion;
    /* ignoring this for now */
    uint16_t Unknown;

    std::vector<SubRecord*> subrecords;

    Record(){};

    Record(std::ifstream& stream)
    {
        stream.read(reinterpret_cast<char*>(&this->Type), 4);
        stream.read(reinterpret_cast<char*>(&this->DataSize), sizeof(uint32_t));
        stream.read(reinterpret_cast<char*>(&this->Flags), sizeof(uint32_t));
        stream.read(reinterpret_cast<char*>(&this->ID), sizeof(uint32_t));
        stream.read(reinterpret_cast<char*>(&this->CreationKitRevision), sizeof(uint32_t));
        stream.read(reinterpret_cast<char*>(&this->FormVersion), sizeof(uint16_t));
        stream.read(reinterpret_cast<char*>(&this->Unknown), sizeof(uint16_t));

        // dummy read - stream offset
        stream.seekg(DataSize, std::ios_base::cur);
    }

    virtual Record* clone(std::ifstream& stream) = 0;

    static Record* ParseRecord(std::ifstream& stream, uint32_t type);
    static std::unordered_map<uint32_t, Record*> prototypes;
};

class Group {
public:

  /* TBD */
    uint32_t ID;

    uint8_t Type[4]; // always GRUP
    uint32_t GroupSize;
    uint8_t Label[4];
    int32_t GroupType;
    /*
     * Stamp format:
     * First byte -> day of month
     * Second byte -> number of months, uknown from what
     */
    uint16_t DateStamp;
    uint8_t Unknown[6];
    /*
     * Data size is: GroupSize - 24
     */
    uint8_t Data[];

    Group(std::ifstream& stream)
    {
        stream.read(reinterpret_cast<char*>(&Type), 4);
        stream.read(reinterpret_cast<char*>(&GroupSize), 4);
        stream.read(reinterpret_cast<char*>(&Label), 4);
        stream.read(reinterpret_cast<char*>(&GroupType), 4);
    }
};
