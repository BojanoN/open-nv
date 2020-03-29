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

    Record(std::ifstream& stream) : stream(stream)
    {
        u_int8_t type[4];
        // ...
    }

private:
    std::ifstream& stream;
};

class Group {
public:
    /* TBD */
    uint32_t ID;

    uint8_t Type[4];
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
    }
};
