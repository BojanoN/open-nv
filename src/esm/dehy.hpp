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
    std::string editorId;
    StageData   data;

    DehydrationStage(ESMReader& reader);
};
}
