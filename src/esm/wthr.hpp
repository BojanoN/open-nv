#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct TimeOfDayColors {
    rgba sunrise;
    rgba day;
    rgba sunset;
    rgba night;
    rgba noon;
    rgba midnight;
};

struct ColorsByType {
    TimeOfDayColors skyUpper;
    TimeOfDayColors fog;
    TimeOfDayColors unused;
    TimeOfDayColors ambient;
    TimeOfDayColors sunlight;
    TimeOfDayColors sun;
    TimeOfDayColors stars;
    TimeOfDayColors skyLower;
    TimeOfDayColors horizon;
    TimeOfDayColors unused2;
};

struct FogDistance {
    float dayNear;
    float dayFar;
    float nightNear;
    float nightFar;
    float dayPower;
    float nightPower;
};

enum class WeatherClassification : uint8_t {
    None     = 0,
    Pleasant = 1,
    Cloudy   = 2,
    Rainy    = 4,
    Snow     = 8
};

struct __attribute__((packed)) WeatherData {
    uint8_t windSpeed;
    uint8_t upperCloudSpeed;
    uint8_t lowerCloudSpeed;

    uint8_t transitionDelta;
    uint8_t sunGlare;
    uint8_t sunDamage;

    uint8_t precipBeginFadeIn;
    uint8_t precipEndFadeOut;

    uint8_t thunderBeginFadeIn;
    uint8_t thunderEndFadeOut;
    uint8_t thunderFrequency;

    WeatherClassification type;
    rgb                   lightningColor;
};

enum class WeatherSoundType : uint32_t {
    Default,
    Precipitation,
    Wind,
    Thunder
};
struct WeatherSound {
    formid           sound;
    WeatherSoundType type;
};

struct Weather : public Record {
    std::string editorId;
    formid      sunriseImageModifier;
    formid      dayImageModifier;
    formid      sunsetImageModifier;
    formid      nightImageModifier;
    formid      noonImageModifier;
    formid      midnightImageModifier;
    std::string cloudTextL0;
    std::string cloudTextL1;
    std::string cloudTextL2;
    std::string cloudTextL3;
    // 4 byte unknown omitted
    uint8_t         cloudLayerSpeed[4]; // Value is divided by 2550. Each uint8 is for a different cloud layer.
    TimeOfDayColors cloudLayerColor[4]; // Each structure is for a different cloud layer.
    ColorsByType    colorsType;
    FogDistance     fogDistance;
    // lmao 304 garbage bytes
    WeatherData               data;
    std::vector<WeatherSound> weatherSounds;

    Weather(ESMReader& reader);
};
}
