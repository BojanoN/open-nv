#pragma once

#include "types.hpp"

namespace ESM {

class ESMReader;

struct __attribute__((packed)) RecordHeader {
    uint32_t Type;
    uint32_t DataSize;
    uint32_t Flags;
    formid   ID;
    uint32_t CreationKitRevision;
    uint16_t FormVersion;
    /* ignoring this for now */
    uint16_t Unknown;
};

struct Record {
    uint8_t  Type[4];
    uint32_t Flags;
    formid   ID;

    Record(ESMReader& reader);
    ~Record() {};

    virtual void load(ESMReader& reader) = 0;
};
}
