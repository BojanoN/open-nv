#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

enum class CreatureType : uint8_t {
    Animal,
    MutatedAnimal,
    MutatedInsect,
    Abomination,
    SuperMutant,
    FeralGhoul,
    Robot,
    Giant
};

struct __attribute__((packed)) CreatureData {
    CreatureType type;
    uint8_t      combatSkill;
    uint8_t      magicSkill;
    uint8_t      stealthSkill;
    uint16_t     health;
    uint8_t      unused[2];
    uint16_t     damage;
    uint8_t      strenght;
    uint8_t      perception;
    uint8_t      endurance;
    uint8_t      charisma;
    uint8_t      intelligence;
    uint8_t      agility;
    uint8_t      luck;
};

enum class SoundPart : uint32_t {
    LeftFoot,
    RightFoot,
    LeftBackFoot,
    RighBackFoot,
    Idle,
    Aware,
    Attack,
    Hit,
    Death,
    Weapon,
    MovementLoop,
    ConsciousLoop,
    Auxiliary1,
    Auxiliary2,
    Auxiliary3,
    Auxiliary4,
    Auxiliary5,
    Auxiliary6,
    Auxiliary7,
    Auxiliary8,
    Jump,
    Loop
};

struct SoundTypeSubrecord {
    SoundPart type;
    formid    sound;
    uint8_t   soundChance;
};

struct Creature : public Record {
    std::string                                 editorId;
    ObjectBounds                                objectBounds;
    std::string                                 name;
    ModelData                                   modelData;
    std::vector<formid>                         actorEffects; //SPEL
    formid                                      unarmedAttackEffect;
    AttackAnimation                             unarmedAttackAnimation;
    std::vector<std::string>                    modelList;
    std::vector<uint8_t>                        textureFileHashes;
    CREA_ACBS                                   configuration;
    std::vector<ActorFaction>                   factions;
    formid                                      deathItem; //LVLI
    formid                                      voice; //VTCK
    formid                                      templ; //NPC_ or LVLN
    formid                                      race;
    DestructionData                             destructionData;
    formid                                      script;
    std::vector<std::pair<Item, ItemExtraData>> items;
    AIData                                      aiData;
    std::vector<formid>                         packages;
    std::vector<std::string>                    animations;
    CreatureData                                data;
    uint8_t                                     attackReach;
    formid                                      combatStyle;
    formid                                      bodyPart; // BPTD
    float                                       turningSpeed;
    float                                       baseScale;
    float                                       footWeight;
    ImpactMaterialType                          impactMaterialType;
    SoundLevel                                  soundLevel;
    formid                                      inheritSound; // CREA
    std::vector<SoundTypeSubrecord>             sounds;
    formid                                      impactDataset; // IPDS
    formid                                      meleeWeaponsList; // FLST

    Creature(ESMReader& reader);
};
}
