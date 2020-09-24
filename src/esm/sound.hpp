#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace ESM {
class ESMReader;

enum SoundFlags : uint32_t {
    RANDOM_FREQUENCY_SHIFT   = 0x1,
    PLAY_AT_RANDOM           = 0x2,
    ENVIRONMENT_IGNORED      = 0x4,
    RANDOM_LOCATION          = 0x8,
    LOOP                     = 0x10,
    MENU_SOUND               = 0x20,
    TWO_DIMENSIONAL          = 0x40,
    THREE_SIXTY_LFE          = 0x80,
    DIALOGUE_SOUND           = 0x100,
    ENVELOPE_FAST            = 0x200,
    ENVELOPE_SLOW            = 0x400,
    TWO_DIMENSIONAL_RADIUS   = 0x800,
    MUTE_WHEN_SUBMERGED      = 0x1000,
    START_AT_RANDOM_POSITION = 0x2000
};

struct SoundEx {
    uint8_t  minimumAttenuationDistance; //multiply by 5
    uint8_t  maximumAttenuationDistance; //multiply by 100
    int8_t   frequencyAdjustmentPercentage;
    uint8_t  unused;
    uint32_t flags;
    int16_t  staticAttenuationCdB;
    uint8_t  stopTime;
    uint8_t  startTime;
};

struct SoundData {
    SoundEx soundEx;
    int16_t attenuationPoints[5];
    int16_t reverbAttenuationControl;
    int32_t priority;
    int32_t x;
    int32_t y;
};

struct Sound : public Record {
    ObjectBounds objectBounds;
    std::string  soundFilename;
    uint8_t      randomChangePercentage;
    SoundData    soundData;
    Sound(ESMReader& reader);
};

}
