#pragma once
#include "record.hpp"
#include "subrecords/material_common.hpp"
#include "structs.hpp"
#include "cell.hpp"
#include "../cellblock.hpp"

#include <unordered_map>

namespace ESM {

enum class ParentWorldspaceFlags : uint16_t {
    UseLandData       = 0x0001,
    UseLODData        = 0x0002,
    UseMapData        = 0x0004,
    UseWaterData      = 0x0008,
    UseClimateData    = 0x0010,
    UseImageSpaceData = 0x0020,
};

struct WorldspaceLandData {
    float defaultLandHeight;
    float defaultWaterHeight;
};

struct WorldspaceMapData {
    int32_t useableXSize;
    int32_t useableYSize;
    int16_t nwCellXCoord;
    int16_t nwCellYCoord;
    int16_t seCellXCoord;
    int16_t seCellYCoord;
};
struct WorldspaceMapOffsetData {
    float worldMapScale;
    float cellXOffset;
    float cellYOffset;
};

enum class WorldspaceFlags : uint8_t {
    SmallWorld           = 0x01,
    NoFastTravel         = 0x02,
    NoLODWater           = 0x10,
    NoLODNoise           = 0x20,
    NoNPCFallDamage      = 0x40,
    NeedsWaterAdjustment = 0x80
};

struct WorldspaceCoord {
    float x;
    float y;
};

struct WorldspaceSwappedImpact {
    ImpactMaterialType materialType;
    formid             oldIpct; // IPCT
    formid             newIpct;
};

struct Worldspace : public Record {
    std::string                          name;
    formid                               encounterZone; // ECZN
    formid                               parentWorldspace; // WRLD
    ParentWorldspaceFlags                parentFlags;
    formid                               climate;
    formid                               water;
    formid                               lodWater;
    float                                lodWaterHeight;
    WorldspaceLandData                   landData;
    std::string                          largeIconFilename;
    std::string                          smallIconFilename;
    WorldspaceMapData                    mapData;
    WorldspaceMapOffsetData              mapOffsetData;
    formid                               imageSpace;
    WorldspaceFlags                      flags;
    WorldspaceCoord                      minObjectBounds;
    WorldspaceCoord                      maxObjectBounds;
    formid                               music;
    std::string                          canopyShadow;
    std::string                          waterNoiseTexture;
    std::vector<WorldspaceSwappedImpact> swappedImpact;
    FootstepMaterial                     footstepMaterial;
    std::vector<uint32_t>                offsetData;

    Cell*                                   worldCell;
    std::unordered_map<uint32_t, GameWorld::CellBlock> exteriorWorldBlocks;
    //WorldChildren*                          children;

    Worldspace(ESMReader& reader);
    ~Worldspace();
    void loadCell(ESMReader& reader);
};

}
