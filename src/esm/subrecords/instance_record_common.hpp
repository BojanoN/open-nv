#pragma once
#include <esm/types.hpp>
#include <cstdint>

namespace ESM {

struct LinkedDecal {
    formid reference; //REFR
    //unknown??
};

struct LinkedReferenceColor {
    struct rgba {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;
    };
    rgba linkStartColor;
    rgba linkEndColor;
};

enum ActivateParentsFlags : uint8_t {
    NONE                 = 0,
    PARENT_ACTIVATE_ONLY = 1
};

struct ActivateParent {
    formid reference; //REFR, ACRE, ACHR, PGRE, PMIS
    float  delay;
};

enum EnableParentFlags : uint8_t {
    ENABLE_STATE_OPPOSITE_OF_PARENT = 1,
    POP_IN                          = 2
};

struct EnableParent {
    formid            reference; //PLYR, REFR, ACRE, ACHR, PGRE, PMIS
    EnableParentFlags flags;
    uint8_t           padding[3];
};

struct PositionRotation {
    float xPos;
    float yPos;
    float zPos;
    float xRot;
    float yRot;
    float zRot;
};

}; // namespace ESM