#pragma once
#include "record.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace ESM {

class ESMReader;

enum class PrimitiveType : uint32_t {
    None,
    Box,
    Sphere,
    PortalBox
};

struct Primitive {
    float         xBound;
    float         yBound;
    float         zBound;
    float         red;
    float         green;
    float         blue;
    uint32_t      unknown;
    PrimitiveType type;
};

enum class CollisionLayerValues : uint32_t {
    Unidentified,
    Static,
    AnimStatic,
    Transparent,
    Clutter,
    Weapon,
    Projectile,
    Spell,
    Biped,
    Trees,
    Props,
    Water,
    Trigger,
    Terrain,
    Trap,
    NonCollidable,
    CloudTrap,
    Ground,
    Portal,
    DebrisSmall,
    DebrisLarge,
    AcusticSpace,
    ActorZone,
    ProjectileZone,
    GasTrap,
    ShellCasing,
    TransparentSmall,
    InvisibleWall,
    TransparentSmallAnim,
    DeadBip,
    CharController,
    AvoidBox,
    CollisionBox,
    CameraSphere,
    DoorDetection,
    CameraPick,
    ItemPick,
    LineOfSight,
    PathPick,
    CustomPick1,
    CustomPick2,
    SpellExplosion,
    DroppingPick,
};

struct BoundHalfExtents {
    float X;
    float Y;
    float Z;
};
enum class TeleportDestinationFlags : uint32_t {
    NoAlarm = 1
};

struct TeleportDestination {
    formid                   door; // REFR
    float                    xPos;
    float                    yPos;
    float                    zPos;
    float                    xRot;
    float                    yRot;
    float                    zRot;
    TeleportDestinationFlags flags;
};

enum class MapMarkerFlags : uint8_t {
    Visible     = 1,
    CanTravelTo = 2,
    // Explorer perk
    ShowAllHidden = 4
};

enum class MapMarkerType : uint8_t {
    None,
    City,
    Settlement,
    Encampment,
    NaturalLandmark,
    Cave,
    Factory,
    Monument,
    Military,
    Office,
    TownRuins,
    UrbanRuins,
    SewerRuins,
    Metro,
    Vault,
};

struct __attribute__((packed)) MapMarkerData {
    MapMarkerType type;
    uint8_t       unused;
};

enum class BroadcastRangeType : uint32_t {
    Radius,
    Everywhere,
    WorldspaceAndLinkedInteriors,
    LinkedInteriors,
    CurrentCellOnly
};

struct RadioData {
    float              rangeRadius;
    BroadcastRangeType broadcastType;
    float              staticPercentage;
    formid             positionReference;
};

struct LockData {
    uint8_t level;
    uint8_t unused[3];
    formid  key;
    uint8_t flags; // ??
    uint8_t unknown[11];
};

enum class ActionFlag : uint32_t {
    UseDefault    = 1,
    Activate      = 2,
    Open          = 4,
    OpenByDefault = 8
};

struct NavigationDoorLink {
    formid   navMesh;
    uint32_t unknown;
};

struct PortalData {
    float width;
    float height;
    float xPos;
    float yPos;
    float zPos;
    float quaternion1Rot;
    float quaternion2Rot;
    float quaternion3Rot;
    float quaternion4Rot;
};

struct RoomDataHeader {
    uint16_t linkedRoomsCount;
    uint16_t unused;
};

struct OcclusionPlaneData {
    formid right;
    formid left;
    formid bottom;
    formid top;
};

struct PlacedObject : public Record {
    LinkedReferenceColor linkedReferenceColor;
    formid               base;
    formid               encounterZone;
    // ragdoll data omitted
    // ragdoll biped data omitted
    Primitive                    primitive;
    CollisionLayerValues         collisionLayerValues;
    BoundHalfExtents             halfExtents;
    TeleportDestination          teleportDest;
    MapMarkerFlags               mapMarkerFlags;
    std::string                  mapMarkerName;
    MapMarkerData                mapMarkerData;
    formid                       mapMarkerReputation; // REPU
    formid                       audioLocation; // ALOC
    formid                       target;
    int32_t                      levelModifier;
    PatrolData                   patrolData;
    RadioData                    radioData;
    OwnershipData                ownershipData;
    int32_t                      count;
    float                        radius;
    float                        health;
    float                        radiation;
    float                        charge;
    formid                       ammoType;
    int32_t                      ammoCount;
    std::vector<WaterRenderData> waterReflection;
    std::vector<formid>          litWater;
    std::vector<LinkedDecal>     linkedDecals;
    formid                       linkedReference;
    LinkedReferenceColor         altReferenceColor;
    ActivateParentsFlags         activateParentsFlags;
    std::vector<ActivateParent>  activateParents;
    std::string                  activationPrompt;
    EnableParent                 enableParent;
    formid                       emittance;
    formid                       multiBoundReference;
    ActionFlag                   flags;
    NavigationDoorLink           navDoorLink;
    std::vector<formid>          portalRooms; // REFRs
    PortalData                   portalData;
    uint8_t                      speedTreeSeed;
    RoomDataHeader               roomHeader;
    PortalData                   occlusionPlane;
    OcclusionPlaneData           linkedOcclusionPlanes;
    float                        scale;
    formid                       linkedRoom;
    bool                         ignoredBySandBox;
    PositionRotation             positionRotation;
    LockData                     lockData;

    PlacedObject(ESMReader& reader);
    ~PlacedObject() {};
};
}
