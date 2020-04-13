#pragma once
#include "types.hpp"
#include "utils.hpp"
#include <cstdio>
#include <cstring>

namespace ESM {

typedef struct __attribute__((packed)) {
    char     Type[4];
    uint32_t DataSize;
    uint32_t Flags;
    formid   ID;
    uint32_t CreationKitRevision;
    uint16_t FormVersion;
    /* ignoring this for now */
    uint16_t Unknown;
} RecordHeader;

typedef struct Record {
public:
    uint8_t  Type[4];
    uint32_t Flags;
    formid   ID;

    Record(FILE* esm_file)
    {
        RecordHeader hdr;

        fread(&hdr, sizeof(RecordHeader), 1, esm_file);

        std::memcpy(this->Type, hdr.Type, 4);
        this->Flags = hdr.Flags;
        this->ID    = hdr.ID;
    };

    ~Record() {};
} Record;
};
