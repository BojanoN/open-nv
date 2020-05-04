#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

class ESMReader;

struct DehydrationData {
    uint32_t triggerThreshold;
    formid   actorEffect; // SPEL
};

struct DehydrationStage : public Record {
    std::string     editorId;
    DehydrationData data;

    DehydrationStage(ESMReader& reader);
};
}
