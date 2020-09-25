#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "subrecords/modeldata.hpp"
#include "subrecords/destructiondata.hpp"
#include "subrecords/sound_common.hpp"
#include "structs.hpp"
#include "util/enumflags.hpp"

namespace ESM {
class ESMReader;
enum class ProjectileFlags : uint16_t {
    Hitscan       = 0x0001,
    Explosion     = 0x0002,
    AltTrigger    = 0x0004,
    MuzzleFlash   = 0x0008,
    CanBeDisabled = 0x0020,
    CanBePickedUp = 0x0040,
    Supersonic    = 0x0080,
    PinsLimbs     = 0x0100,
    PassThrough   = 0x0200,
    Detonates     = 0x0400,
    Rotation      = 0x0800
};

enum class ProjectileType : uint16_t {
    Missile        = 1,
    Lobber         = 2,
    Beam           = 4,
    Flame          = 8,
    ContinuousBeam = 16
};

struct ProjectileData {
    ProjectileFlags flags;
    ProjectileType  type;
    float           gravity;
    float           speed;
    float           range;
    formid          light; // LIGH
    formid          muzzleFlashLight; // LIGH
    float           tracerChance;
    float           explosionProximity;
    float           explosionTimer;
    formid          explosion; // EXPL
    formid          sound; // SOUND
    float           muzzleFlashDuration;
    float           fadeDuration;
    float           impactForce;
    formid          countdownSound;
    formid          disableSound;
    formid          defaultWeaponSource;
    float           xRot;
    float           yRot;
    float           zRot;
    float           bouncyMultiplier;
};

struct Projectile : public Record {
    ObjectBounds         objectBounds;
    std::string          name;
    ModelData            modelData;
    DestructionData      destructionData;
    ProjectileData       projectileData;
    std::string          muzzleFlashModel;
    std::vector<uint8_t> muzzleFlashModelHash;
    SoundLevel           soundLevel;

    Projectile(ESMReader& reader);
};
}

ENABLE_BITWISE_OPERATORS(ESM::ProjectileType);
