#pragma once

#include <cstdint>
#include <fstream>
#include <vector>

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
    uint16_t Unknown;
    std::vector<SubRecord> Data;

    Record(std::ifstream& stream)
    {
        stream.read(reinterpret_cast<char*>(&Type), 4);
        stream.read(reinterpret_cast<char*>(&DataSize), 4);
        stream.read(reinterpret_cast<char*>(&Flags), 4);
        stream.read(reinterpret_cast<char*>(&ID), 4);
        stream.read(reinterpret_cast<char*>(&CreationKitRevision), 4);
        stream.read(reinterpret_cast<char*>(&FormVersion), 2);
        stream.read(reinterpret_cast<char*>(&Unknown), 2);

        // dummy data read to move the position in stream to the end of the record
        stream.seekg(DataSize, std::ios_base::cur);
    }
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
