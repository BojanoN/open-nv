#pragma once
#include <sys/types.h>
#include <vector>

class Record
{
public:
    u_int8_t Type[4];
    u_int32_t DataSize;
    u_int32_t Flags;
    u_int32_t ID;
    u_int32_t CreationKitRevision;
    u_int16_t FormVersion;
    u_int16_t Unknown;
    std::vector<SubRecord> Data;

    Record(std::ifstream &stream) : stream(stream)
    {
        u_int8_t type[4];
        // ...
    }

private:
    std::ifstream &stream;
};

class SubRecord
{
public:
    u_int8_t Type[4];
    u_int16_t DataSize;
    u_int8_t *Data; // u_int8_t Data[DataSize] for now, later a vector
};