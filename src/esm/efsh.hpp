#pragma once
#include "record.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace ESM {

class ESMReader;

enum class EffectShaderFlags : uint8_t {
    NoMembraneShader       = 0x01,
    NoParticleShader       = 0x08,
    EdgeEffectInverse      = 0x10,
    MembraneShaderSkinOnly = 0x20
};

enum class BlendMode : uint32_t {
    Unknown,
    Zero,
    One,
    SourceColor,
    SourceInverseColor,
    SourceAlpha,
    SourceInertedAlpha,
    DestinationAlpha,
    DestinationInvertedAlpha,
    DestinationColor,
    DestimationInverseColor,
    SourceAlphaSAT,
};

enum class BlendOperationValues : uint32_t {
    Unknown,
    Add,
    Subtract,
    ReverseSubtract,
    Minimum,
    Maximum
};

enum class ZTestFunctionValue : uint32_t {
    EqualTo            = 3,
    Normal             = 4,
    GreaterThan        = 5,
    GreaterThanOrEqual = 7,
    AlwaysShow         = 8
};

struct FillTextureEffect {
    rgba  color;
    float alphaFadeInTime;
    float fullAlphaTime;
    float alphaFadeOutTime;
    float persistentAlphaRatio;
    float alphaPulseAmplitude;
    float alphaPulseFrequency;
    float textureAnimationSpeedU;
    float textureAnimationSpeedV;
};

struct EdgeEffect {
    float fallOff;
    rgba  color;
    float alphaFadeInTime;
    float fullAlphaTime;
    float alphaFadeOutTime;
    float persistentAlphaRatio;
    float alphaPulseAmplitude;
    float alphaPulseFrequency;
    float textEffectfullAlphaRatio;
    float fullAlphaRatio;
};

struct MembraneShader {
    BlendMode            sourceBlendMode;
    BlendOperationValues operationValues;
    ZTestFunctionValue   zTestValues;

    FillTextureEffect textureEffect;
    EdgeEffect        edgeEffect;
    BlendMode         destBlendMode;
};

struct ParticleShader {
    BlendMode            sourceBlendMode;
    BlendOperationValues operationValues;
    ZTestFunctionValue   zTestValues;
    BlendMode            destBlendMode;
    float                particleBirthRampUpTime;
    float                fullParticleBirthTime;
    float                particleBirthRampDownTime;
    float                fullParticleBirthRatio;
    float                persistentParticleBirthRatio;
    float                particleLifetime;
    float                particleLifetimeTol;
    float                initialSpeedAlongNormal;
    float                accelAlongNormal;
    float                initialVelocity1;
    float                initialVelocity2;
    float                initialVelocity3;
    float                acceleration1;
    float                acceleration2;
    float                acceleration3;
    float                scaleKey1;
    float                scaleKey2;
    float                scaleKey1Time;
    float                scaleKey2Time;
    rgba                 colorKey1;
    rgba                 colorKey2;
    rgba                 colorKey3;
    float                colorKey1Alpha;
    float                colorKey2Alpha;
    float                colorKey3Alpha;
    float                colorKey1Time;
    float                colorKey2Time;
    float                colorKey3Time;
    float                initialSpeedAlongNormalTol;
    float                initialRotationDeg;
    float                initialRotationDegTol;
    float                initialRotationSpeed;
    float                initialRotationSpeedTol;
};

struct Holes {
    float startTime;
    float endTime;
    float startValue;
    float endValue;
};

struct AddonModels {
    float fadeInTime;
    float fadeOutTime;
    float scaleStart;
    float scaleEnd;
    float scaleInTime;
    float scaleOutTime;
};

struct __attribute__((packed)) EffectShaderData {
    EffectShaderFlags flags;
    uint8_t           unused[3];
    MembraneShader    membraneShader;
    ParticleShader    particleShader;
    formid            addonModels;
    Holes             holes;
    float             edgeWidth;
    rgba              edgeColor;
    float             explosionWindSpeed;
    uint32_t          textureCountU;
    uint32_t          textureCountV;
    AddonModels       addonModelsData;
};

struct EffectShader : public Record {
    std::string      editorId;
    std::string      fillTexture;
    std::string      particleShaderTexture;
    std::string      holesTexture;
    EffectShaderData data;

    EffectShader(ESMReader& reader);
};
};
