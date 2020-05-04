#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

enum class ImageSpaceAdapterFlags : uint32_t {
    ANIMATABLE = 0x00000001
};

enum class RadialBlurFlags : uint32_t {
    USE_TARGET = 0x00000001
};

enum class DepthOfFieldFlags : uint32_t {
    USE_TARGET = 0x00000001
};

struct ImageSpaceAdapterCounts {
    ImageSpaceAdapterFlags flags;
    float duration;
    uint32_t eyeAdaptSpeedMult;
    uint32_t eyeAdaptSpeedAdd;
    uint32_t bloomBlurRadiusMult;
    uint32_t bloomBlurRadiusAdd;
    uint32_t bloomThresholdMult;
    uint32_t bloomThresholdAdd;
    uint32_t bloomScaleMult;
    uint32_t bloomScaleAdd;
    uint32_t targetLUMMinMult;
    uint32_t targetLUMMinAdd;
    uint32_t targetLUMMaxMult;
    uint32_t targetLUMMaxAdd;
    uint32_t sunlightScaleMult;
    uint32_t sunlightScaleAdd;
    uint32_t skyScaleMult;
    uint32_t skyScaleAdd;
    uint8_t unknown[72]; // \_(:_:)_/
    uint32_t cinematicSaturationMult;
    uint32_t cinematicSaturationAdd;
    uint32_t cinematicBrightnessMult;
    uint32_t cinematicBrightnessAdd;
    uint32_t cinematicContrastMult;
    uint32_t cinematicContrastAdd;
    uint8_t  unknown_2[8];
    uint32_t tintColor;
    uint32_t blurRadius;
    uint32_t doubleVisionStrength;
    uint32_t radialBlurStrength;
    uint32_t radialBlurRampUp;
    uint32_t radialBlurStart;
    RadialBlurFlags radialBlurFlags;
    uint32_t radialBlurCenterX;
    uint32_t radialBlurCenterY;
    uint32_t depthOfFieldStrength;
    uint32_t depthOfFieldDistance;
    uint32_t depthOfFieldRange;
    DepthOfFieldFlags depthOfFieldFlags;
    uint32_t radialBlurRampDown;
    uint32_t radialBlurDownStart;
    uint32_t fadeColor;
    uint32_t motionBlurStrength;
};

struct ImageSpaceAdapterTime {
    float time;
    float value;
};

struct ImageSpaceAdapterColor {
    float time;
    rgbf   color;
    float alpha;
};

struct ImageSpaceAdapter : public Record {
    std::string                         editorId;
    ImageSpaceAdapterCounts             counts;
    std::vector<ImageSpaceAdapterTime>  blurRadius;
    std::vector<ImageSpaceAdapterTime>  doubleVisionStrength;
    std::vector<ImageSpaceAdapterColor> tintColor;
    std::vector<ImageSpaceAdapterColor> fadeColor;
    std::vector<ImageSpaceAdapterTime>  radialBlurStrength;
    std::vector<ImageSpaceAdapterTime>  radialBlurRampUp;
    std::vector<ImageSpaceAdapterTime>  radialBlurStart;
    std::vector<ImageSpaceAdapterTime>  radialBlurRampDown;
    std::vector<ImageSpaceAdapterTime>  radialBlurDownStart;
    std::vector<ImageSpaceAdapterTime>  depthOfFieldStrength;
    std::vector<ImageSpaceAdapterTime>  depthOfFieldDistance;
    std::vector<ImageSpaceAdapterTime>  depthOfFieldRange;
    std::vector<ImageSpaceAdapterTime>  motionBlurStrength;
    std::vector<ImageSpaceAdapterTime>  eyeAdaptSpeedMult;
    std::vector<ImageSpaceAdapterTime>  eyeAdaptSpeedAdd;
    std::vector<ImageSpaceAdapterTime>  bloomBlurRadiusMult;
    std::vector<ImageSpaceAdapterTime>  bloomBlurRadiusAdd;
    std::vector<ImageSpaceAdapterTime>  bloomThresholdMult;
    std::vector<ImageSpaceAdapterTime>  bloomThresholdAdd;
    std::vector<ImageSpaceAdapterTime>  bloomScaleMult;
    std::vector<ImageSpaceAdapterTime>  bloomScaleAdd;
    std::vector<ImageSpaceAdapterTime>  targetLUMMinMult;
    std::vector<ImageSpaceAdapterTime>  targetLUMMinAdd;
    std::vector<ImageSpaceAdapterTime>  targetLUMMaxMult;
    std::vector<ImageSpaceAdapterTime>  targetLUMMaxAdd;
    std::vector<ImageSpaceAdapterTime>  sunlightScaleMult;
    std::vector<ImageSpaceAdapterTime>  sunlightScaleAdd;
    std::vector<ImageSpaceAdapterTime>  skyScaleMult;
    std::vector<ImageSpaceAdapterTime>  skyScaleAdd;
    //9 unknown mult and add pairs
    std::vector<ImageSpaceAdapterTime>  cinematicSaturationMult;
    std::vector<ImageSpaceAdapterTime>  cinematicSaturationAdd;
    std::vector<ImageSpaceAdapterTime>  cinematicBrightnessMult;
    std::vector<ImageSpaceAdapterTime>  cinematicBrightnessAdd;
    std::vector<ImageSpaceAdapterTime>  cinematicContrastMult;
    std::vector<ImageSpaceAdapterTime>  cinematicContrastAdd;
    //1 unknown pair
    formid                             soundIntro;
    formid                             soundOutro;

    ImageSpaceAdapter(ESMReader& reader);
};
};
