#pragma once
#include "record.hpp"
#include "structs.hpp"

#include <variant>

namespace ESM {

enum class AmmoFlags : uint8_t {
    IGNORES_NORMAL_WEAPON_RESISTANCE = 0x01,
    NON_PLAYABLE                     = 0x02
};

class ESMReader;

struct __attribute__((packed)) AmmoData {
    float     speed;
    AmmoFlags flag;
    uint8_t   unused[3];
    int32_t   value;
    uint8_t   clipRounds;
};

struct __attribute__((packed)) AmmoProjectileData {
    uint32_t projectilesPerShot;
    formid   projectile;
    float    weight;
    formid   consumedAmmo;
    float    consumedPercentage;
};

struct __attribute__((packed)) AmmoNonProjectileData {
    // This subrecord is not documented, further investigation needed
    formid   projectile;
    uint32_t projectilesPerShot;
    float    weight;
};

struct Ammo : public Record {
    std::string                                             editorId;
    ObjectBounds                                            objectBounds;
    std::string                                             name;
    ModelData                                               modelData;
    std::string                                             largeIconFilename;
    std::string                                             smallIconFilename;
    formid                                                  script;
    DestructionData                                         destructionData;
    formid                                                  pickUpSound;
    formid                                                  dropSound;
    AmmoData                                                data;
    std::variant<AmmoProjectileData, AmmoNonProjectileData> projectileData;
    std::string                                             shortName;
    std::string                                             abbreviation;
    formid                                                  ammoEffect;

    Ammo(ESMReader& reader);
};
};
