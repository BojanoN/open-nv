#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct AddonNode : public Record {

    ObjectBounds objectBounds;
    ModelData    modelData;
    int32_t      nodeIndex;
    formid       ragdoll;
    uint32_t     unknown; //SNAM
    uint32_t     masterParticleSystemCap; //first 2 bits used as uin16 value

    AddonNode(ESMReader& reader);
};

};
