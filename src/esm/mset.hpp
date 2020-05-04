#pragma once
#include "record.hpp"
#include "types.hpp"

namespace ESM {

class ESMReader;

enum class MediaSetType : int32_t {
    NoSet         = -1,
    BattleSet     = 0,
    LocationSet   = 1,
    DungeonSet    = 2,
    IncidentalSet = 3
};

enum class MediaSetEnableFlags : uint8_t {
    DayOuter    = 0x01,
    DayMiddle   = 0x02,
    DayInner    = 0x04,
    NightOuter  = 0x08,
    NightMiddle = 0x10,
    NightInner  = 0x20,
};

struct MediaSet : public Record {
    std::string  editorId;
    std::string  name;
    MediaSetType type;
    std::string  loopBattleDayOuter;
    std::string  exploreDayMiddle;
    std::string  suspenseDayInner;
    std::string  nightOuter;
    std::string  nightMiddle;
    std::string  nightInner;
    float        loopBattleDayOuterdB;
    float        exploreDayMiddledB;
    float        suspenseDayInnerdB;
    float        nightOuterdB;
    float        nightMiddledB;
    float        nightInnerdB;

    float               dayOuterBoundPercent;
    float               dayMiddleBoundPercent;
    float               dayInnerBoundPercent;
    float               nightOuterBoundPercent;
    float               nightMiddleBoundPercent;
    float               nightInnerBoundPercent;
    MediaSetEnableFlags enableFlags;
    float               waitTime;
    float               loopFadeOut;
    float               recoveryTime;
    float               nightTimeMax;
    float               unknown;
    formid              intro;
    formid              outro;

    MediaSet(ESMReader& reader);
};
}
