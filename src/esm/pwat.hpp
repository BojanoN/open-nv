#pragma once
#include "record.hpp"
#include "reader.hpp"
#include "subrecords/objectbounds.hpp"
#include "structs.hpp"

namespace ESM {

enum class PlaceableWaterFlags : uint32_t {
    REFLECTS   = 0x01,
    REFLECTS_ACTORS = 0x02,
    REFLECTS_LAND = 0x04,
    REFLECTS_LOD_LAND = 0x08,
    REFLECTS_LOD_BUILDINGS = 0x10,
    REFLECTS_TREES = 0x20,
    REFLECTS_SKY = 0x40,
    REFLECTS_DYNAMIC_OBJECTS = 0x80,
    REFLECTS_DEAD_BODIES = 0x100,

    REFRACTS = 0x200,
    REFRACTS_ACTORS = 0x400,
    REFRACTS_LAND = 0x800,

    REFRACTS_DYNAMIC_OBJECTS = 0x10000,
    REFRACTS_DEAD_BODIES = 0x20000,
    SILLHOUETTE_REFLECTIONS = 0x40000,

    DEPTH = 0x10000000,
    OBJECT_TEXTURE_COORDINATES = 0x20000000,
    NO_UNDERWATER_FOG = 0x80000000
};


struct PlaceableWaterData {
    PlaceableWaterFlags flags;
    formid   water;
};

struct PlaceableWater : public Record {
    ObjectBounds    objectBounds;
    ModelData       modelData;
    PlaceableWaterData data;

    PlaceableWater(ESMReader& reader);
};
}
