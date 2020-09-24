#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "subrecords/destructiondata.hpp"
#include "structs.hpp"

namespace ESM {
class ESMReader;

enum class DoorFlags : uint8_t {
    UNKNOWN        = 0x01,
    AUTOMATIC_DOOR = 0x02,
    HIDDEN         = 0x04,
    MINIMAL_USE    = 0x08,
    SLIDING_DOOR   = 0x10

};

struct Door : public Record {
    ObjectBounds    objectBounds;
    std::string     name;
    ModelData       modelData;
    formid          script;
    DestructionData destructionData;
    formid          openSound;
    formid          closeSound;
    formid          loopingSound;
    DoorFlags       flag;

    Door(ESMReader& reader);
};

}
