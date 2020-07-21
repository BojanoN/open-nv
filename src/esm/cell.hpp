#pragma once
#include "../gamedata.hpp"
#include "childrecords.hpp"
#include "record.hpp"
#include "structs.hpp"

#include <cstdint>
#include <sstream>
#include <string>

namespace ESM {

enum class CellFlags : uint8_t {
    IS_INTERIOR_CELL            = 0x01,
    HAS_WATER                   = 0x02,
    INVERT_FAST_TRAVEL_BEHAVIOR = 0x04,
    NO_LOD_WATER                = 0x08,
    UNKNOWN                     = 0x10,
    PUBLIC_PLACE                = 0x20,
    HAND_CHANGED                = 0x40,
    BEHAVE_LIKE_EXTERIOR        = 0x80
};

enum class ForceHideLandFlags : uint32_t {
    QUAD_1,
    QUAD_2,
    QUAD_3,
    QUAD_4,
};

struct CellGrid {
    int32_t            x;
    int32_t            y;
    ForceHideLandFlags forceHideLandFlags;
};

struct CellLighting {
    rgba    ambientColor;
    rgba    directionalColor;
    rgba    fogColor;
    float   fogNear;
    float   fogFar;
    int32_t directionalRotationXY;
    int32_t directionalRotationZ;
    float   directionalFade;
    float   fogClipDistance;
    float   fogPower;
};

enum class CellLightTemplateInheritFlags : uint32_t {
    AMBIENT_COLOR,
    DIRECTIONAL_COLOR,
    FOG_COLOR,
    FOG_NEAR,
    FOG_FAR,
    DIRECTIONAL_ROTATION,
    DIRECTIONAL_FADE,
    FOG_CLIP_DISTANCE,
    FOG_POWER
};

struct CellLightTemplate {
    formid                        templ; //LGTM or null
    CellLightTemplateInheritFlags inheritFlags;
};

class CellChildren {

    GameWorld::GameData<PlacedObject>   placedObjects; //REFR
    GameWorld::GameData<PlacedCreature> placedCreatures; //ACRE
    GameWorld::GameData<PlacedGrenade>  placedGrenades; //PGRE
    //GameWorld::GameData<PlacedMissile>  placedMissiles; //PMIS
    GameWorld::GameData<PlacedNPC>      placedNPCs; //ACHR
    GameWorld::GameData<NavigationMesh> navigationMeshes; //NAVM
    GameWorld::GameData<Landscape>      landscapes; //LAND

    std::unordered_map<uint32_t, GameWorld::GameDataBase*> typeMap;

public:
    CellChildren();
    void load(ESMReader& reader);
};

struct Cell : public Record {

    std::string         name;
    CellFlags           flags;
    CellGrid            grid;
    CellLighting        lighting;
    FootstepMaterial    footstepMaterial;
    CellLightTemplate   lightTemplate;
    float               waterHeight;
    std::string         waterNoiseTexture;
    std::vector<formid> regions; //REGN
    formid              imageSpace; //IMGS
    uint8_t             unknown;
    formid              encounterZone; //ECZN
    formid              climate; //CLMT
    formid              water; //WATR
    formid              owner; //FACT, ACHR, CREA, NPC_
    int32_t             factionRank; //"ownership data"
    formid              acousticSpace; //ASPC
    uint8_t             unused;
    formid              musicType; //MUSC

    class CellChildren* persistentChildren;
    class CellChildren* temporaryChildren;
    class CellChildren* visibleDistantChildren;

    Cell(ESMReader& reader);
    ~Cell();
    class CellChildren* getChildren(uint32_t groupType);
};

};
