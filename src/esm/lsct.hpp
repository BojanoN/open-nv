#pragma once
#include "record.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace ESM {

class ESMReader;

enum class LoadingScreenTypeType : uint32_t {
    None,
    XPProgress,
    Objective,
    Tip,
    Stats
};

enum class FontAlignment : uint32_t {
    Left   = 1,
    Center = 2,
    Right  = 4
};

struct __attribute__((packed)) LoadingScreenTypeData {
    LoadingScreenTypeType type;
    uint32_t              x;
    uint32_t              y;
    uint32_t              width;
    uint32_t              height;
    float                 orientation;
    uint32_t              font1;
    rgbf                  font1Color;
    FontAlignment         font1Align;
    uint8_t               unknown[20];
    uint32_t              font2;
    rgbf                  font2Color;
    uint8_t               unknown2[4];
    uint32_t              stats; // ??
};

struct LoadingScreenType : public Record {
    LoadingScreenTypeData data;

    LoadingScreenType(ESMReader& reader);
};
}
