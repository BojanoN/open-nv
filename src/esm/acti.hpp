#pragma once
#include "record.hpp"
#include "structs.hpp"

#include <cstdint>
#include <string>
#include <unordered_set>

namespace ESM {

struct Activator : public Record {

    ObjectBounds    objectBounds;
    std::string     name;
    ModelData       modelData;
    formid          script;
    DestructionData destructionData;
    formid          soundLooping;
    formid          soundActivation;
    formid          radioTemplate;
    formid          radioStation;
    formid          waterType;
    std::string     activationPrompt;

    Activator(ESMReader& reader);
};

};
