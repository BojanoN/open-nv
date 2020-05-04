#pragma once
#include "dehy.hpp"

namespace ESM {

class ESMReader;

struct HungerStage : public DehydrationStage {
    HungerStage(ESMReader& reader)
        : DehydrationStage(reader)
    {
    }
};
}
