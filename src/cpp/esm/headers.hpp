#pragma once
#include "types.hpp"

namespace ESM {

class ESMReader;

struct RecordHeader {
    uint32_t  type;
    uint32_t dataSize;
    uint32_t flags;
    formid   id;
    uint32_t creationKitRevision;
    uint16_t formVersion;
    /* ignoring this for now */
    uint16_t unknown;
};

struct GroupHeader {
    uint32_t type;
    uint32_t groupSize;
    uint32_t label;
    int32_t  groupType;
    uint16_t stamp;
    uint8_t  unknown[6];
};

#pragma pack(1)
struct SubrecordHeader {
    uint32_t  type;
    uint16_t dataSize;
} ;
#pragma options align=reset


}
