#pragma once
#include "scriptdata.hpp"

namespace ESM {

struct PatrolData {
    float      idleTime;
    formid     idleAnimation;
    ScriptData patrolScript;
    formid     dialogue;
};

}; // namespace ESM