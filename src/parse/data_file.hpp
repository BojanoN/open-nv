#pragma once
#include <sys/types.h>

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
    u_int8_t *Data; // u_int8_t Data[DataSize]
};

class SubRecord
{
    u_int8_t Type[4];
    u_int16_t DataSize;
    u_int8_t *Data; // u_int8_t Data[DataSize]
};