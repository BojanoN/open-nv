#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

class ESMReader;

struct StageData {
    uint32_t triggerThreshold;
    formid   actorEffect; // SPEL
};

struct DehydrationStage : public Record {
    StageData   data;

    DehydrationStage(ESMReader& reader);
};
}
