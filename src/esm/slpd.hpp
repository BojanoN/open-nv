#pragma once
#include "dehy.hpp"

namespace ESM {

class ESMReader;

struct SleepDeprivationStage : public DehydrationStage {
    SleepDeprivationStage(ESMReader& reader)
        : DehydrationStage(reader)
    {
    }
};
}
