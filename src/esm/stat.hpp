#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {
class ESMReader;

enum class BushPassthroughSound : int8_t {
    None  = -1,
    BushA = 0,
    BushB = 1,
    BushC = 2,
    BushD = 3,
    BushE = 4,
    BushF = 5,
    BushG = 6,
    BushH = 7,
    BushI = 8,
    BushJ = 9,
};

struct Static : public Record {
    std::string          editorId;
    ObjectBounds         objectBounds;
    ModelData            model;
    BushPassthroughSound sound;
    formid               loopingSound;

    Static(ESMReader& reader);
};
}
