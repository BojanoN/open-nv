#pragma once
#include "../reader.hpp"
#include <cstdint>
#include <vector>
#include <string>

namespace ESM {

struct DestructionHeader {
    int32_t health;
    uint8_t count;
    uint8_t flags;
    uint8_t unused[2];
};

struct DestructionStageData {
    uint8_t healthPercentage;
    uint8_t index;
    uint8_t damageStage;
    uint8_t flags;
    int32_t selfDamagePerSecond;
    formid  explosion;
    formid  debris;
    int32_t debrisCount;
};

struct DestructionStage {
    DestructionStageData stageData;
    std::string          modelFilename;
    std::vector<uint8_t> textureHashes;
};

struct DestructionData {
    DestructionHeader             header;
    std::vector<DestructionStage> stages;
    static void                   load(ESMReader& reader, DestructionData& destData);

private:
    const static constexpr ESMType types[] = { ESMType::DSTD, ESMType::DMDL, ESMType::DMDT, ESMType::DSTF };
    static bool                    isInCollection(uint32_t type);
};

}; // namespace ESM