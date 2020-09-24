#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "structs.hpp"

namespace ESM {
class ESMReader;

enum class FromWaterType : uint32_t {
    ABOVE_AT_LEAST       = 0,
    ABOVE_AT_MOST        = 1,
    BELOW_AT_LEAST       = 2,
    BELOW_AT_MOST        = 3,
    EITHER_AT_LEAST      = 4,
    EITHER_AT_MOST       = 5,
    EITHER_AT_MOST_ABOVE = 6,
    EITHER_AT_MOST_BELOW = 7,
};

enum class GrassDataFlags : uint8_t {
    VERTEX_LIGHTING = 0x01,
    UNIFORM_SCALING = 0x02,
    FIT_TO_SLOPE    = 0x04,
};

struct __attribute__((packed)) GrassData {
    uint8_t density;
    uint8_t minSlope;
    uint8_t maxSlope;
    uint8_t unused;

    uint16_t      unitFromWaterAmount;
    uint8_t       unused2[2];
    FromWaterType waterType;

    float          positionRange;
    float          heightRange;
    float          colorRange;
    float          wavePeriod;
    GrassDataFlags flag;
    uint8_t        unused3[3];
};

struct Grass : public Record {
    ObjectBounds objectBounds;
    ModelData    modelData;
    GrassData    data;

    Grass(ESMReader& reader);
};

}
