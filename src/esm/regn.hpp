#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct RegionPointListData {
    float x;
    float y;
};

struct RegionArea {
    uint32_t                         edgeFallOff;
    std::vector<RegionPointListData> data;
};

enum class RegionDataObjectFlags : uint8_t {
    ConformToSlope = 0x01,
    PaintVertices  = 0x02,
    SizeVariance   = 0x04,
    X              = 0x08,
    Y              = 0x10,
    Z              = 0x20,
    Tree           = 0x40,
    HugeRock       = 0x80
};

struct __attribute__((packed)) RegionDataObject {
    formid                object; // TREE, STAT, or LTEX
    uint16_t              parentIndex;
    uint8_t               unused[2];
    float                 density;
    uint8_t               clustering;
    uint8_t               minSlope;
    uint8_t               maxSlope;
    RegionDataObjectFlags flags;
    uint16_t              radiusWithRespectToParent;
    uint16_t              radius;
    uint8_t               unknown[4];
    float                 maxHeight;
    float                 sink;
    float                 sinkVariance;
    float                 sizeVariance;
    uint16_t              xAngleVariance;
    uint16_t              yAngleVariance;
    uint16_t              zAngleVariance;
    uint8_t               unused2[6];
};

struct RegionDataGrasses {
    formid   grass; // GRAS
    uint32_t unknown;
};

enum class RegionDataEntryType : uint32_t {
    Objects  = 2,
    Weather  = 3,
    Map      = 4,
    Land     = 5,
    Grass    = 6,
    Sound    = 7,
    Imposter = 8
};

enum class RegionDataEntryFlag : uint8_t {
    Override = 0x01
};

struct __attribute__((packed)) RegionDataHeader {
    RegionDataEntryType type;
    RegionDataEntryFlag flag;
    uint8_t             unused[3];
};

enum class RegionEntryMusicType : uint32_t {
    Defaut,
    Public,
    Dungeon
};

struct __attribute__((packed)) RegionDataEntryWeatherTypes {
    formid   weather; // WTHR
    uint32_t chance;
    formid   global; // GLOB
};

enum class RegionDataSoundFlags : uint32_t {
    Pleasant = 0x1,
    Cloudy   = 0x2,
    Rainy    = 0x4,
    Snowy    = 0x8
};

struct __attribute__((packed)) RegionDataSound {
    formid               sound;
    RegionDataSoundFlags flags;
    uint32_t             chance;
};

struct __attribute__((packed)) RegionDataEntry {
    RegionDataHeader                         header;
    std::vector<RegionDataObject>            objects;
    std::string                              mapName;
    std::vector<RegionDataGrasses>           grasses;
    RegionEntryMusicType                     musicType;
    formid                                   music; // MUSC
    formid                                   mediaSet;
    formid                                   battleMediaSet;
    std::vector<RegionDataSound>             sounds;
    std::vector<RegionDataEntryWeatherTypes> weatherTypes;
    std::vector<formid>                      imposters; // REFRs

    static void loadRegionData(ESMReader& reader, std::vector<RegionDataEntry>& regionData);
};

struct Region : public Record {
    std::string                  largeIconFilename;
    std::string                  smallIconFilename;
    rgba                         mapColor;
    formid                       worldspace; // WRLD
    std::vector<RegionArea>      areas;
    std::vector<RegionDataEntry> dataEntries;

    Region(ESMReader& reader);
};
}
