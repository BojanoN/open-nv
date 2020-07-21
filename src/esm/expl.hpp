#pragma once
#include "record.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace ESM {

class ESMReader;

struct ExplosionData {
    float      force;
    float      damage;
    float      radius;
    formid     light; // LIGH
    formid     sound1; // SOUN
    uint32_t   flags; // ??
    float      ISRadius;
    formid     impactDataset; // IPDS
    formid     sound2; // SOUN
    float      radiationLevel;
    float      radiationDissipationTime;
    float      radiationRadius;
    SoundLevel soundLevel;
};

struct Explosion : public Record {
    ObjectBounds  objectBounds;
    std::string   name;
    ModelData     modelData;
    formid        objectEffect;
    formid        imageSpaceModifier; // IMAD
    ExplosionData data;
    formid        placedImpactObject;

    Explosion(ESMReader& reader);
};
}
