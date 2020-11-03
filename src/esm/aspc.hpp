#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace ESM {
class ESMReader;

enum EnvironmentType : uint32_t {
    NONE_ENVIRONMENT_TYPE,
    DEFAULT,
    GENERIC,
    PADDED_CELL,
    ROOM,
    BATHROOM,
    LIVINGROOM,
    STONE_ROOM,
    AUDITORIUM,
    CONCERTHALL,
    CAVE,
    ARENA,
    HANGAR,
    CARPETED_HALLWAY,
    HALLWAY,
    STONE_CORRIDOR,
    ALLEY,
    FOREST,
    CITY,
    MOUNTAINS,
    QUARRY,
    PLAIN,
    PARKINGLOT,
    SEWERPIPE,
    UNDERWATER,
    SMALL_ROOM,
    MEDIUM_ROOM,
    LARGE_ROOM,
    MEDIUM_HALL,
    LARGE_HALL,
    PLATE
};

enum IsInterior : uint32_t {
    NO  = 0,
    YES = 1
};

struct AcousticSpace : public Record {
    ObjectBounds objectBounds;
    formid       dawn_default; //SOUN
    formid       afternoon; //SOUN
    formid       dusk; //SOUN
    formid       night; //SOUN
    formid       walla; //SOUN
    uint32_t     wallaTriggerCount;
    formid       regionSound; //REGN
    /*
      see EnvironmentType enum
    */
    EnvironmentType environmentType;
    /*
      see IsInterior enum
    */
    IsInterior isInterior;

    AcousticSpace(ESMReader& reader);
};

}
