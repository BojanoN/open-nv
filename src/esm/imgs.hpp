#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

enum class ImageSpaceFlags : uint8_t {
    SATURATION = 0x01,
    CONTRAST   = 0x02,
    TINT       = 0x04,
    BRIGHTNESS = 0x08
};

struct ImageSpaceData {
    float           hdrEyeAdaptSpeed;
    float           hdrBlurRadius;
    float           hdrBlurPasses;
    float           emissiveMultiplier;
    float           targetLUM;
    float           targetLUMClamp;
    float           brightScale;
    float           brightClamp;
    float           LUMRampNoTex;
    float           LUMRampMin;
    float           LUMRampMax;
    float           sunlightDimmer;
    float           grassDimmer;
    float           treeDimmer;
    float           skinDimmer;
    float           bloomBlurRadius;
    float           bloomAlphaMultInterior;
    float           bloomAlphaMultExterior;
    float           getHitBlurRadius;
    float           getHitBlurDampingConstant;
    float           getHitDampingConstant;
    rgbFloat        nightEyeTintColor;
    float           brightness;
    float           cinematicSaturation;
    float           cinematicContrastAvgLumValue;
    float           cinematicContrastValue;
    rgbFloat        cinematicBrightnessTintColor;
    float           cinematicBrightnessTintValue;
    uint8_t         unused[16];
    ImageSpaceFlags flags;
    uint8_t         padding[3];
    uint8_t         unknown[4];
};

struct ImageSpace : public Record {
    std::string    editorId;
    ImageSpaceData data;
    ImageSpace(ESMReader& reader);
};
};
