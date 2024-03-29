#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "structs.hpp"

namespace ESM {

enum class IdleMarkerFlags : uint8_t {
    RunInSequence = 0x01,
    DoOnce        = 0x04
};

struct IdleMarkerData {
    uint8_t animationCount;
};

struct IdleMarker : public Record {
    ObjectBounds        objectBounds;
    IdleMarkerFlags     flags;
    IdleMarkerData      data;
    float               idleTimer;
    std::vector<formid> animations; // IDLE

    IdleMarker(ESMReader& reader);
};
}
