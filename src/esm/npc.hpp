#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

class ESMReader;

struct __attribute__((packed)) NPCData {
    int32_t baseHealth;
    uint8_t strenght;
    uint8_t perception;
    uint8_t endurance;
    uint8_t charisma;
    uint8_t intelligence;
    uint8_t agility;
    uint8_t luck;
    //possibility of random padding
};

struct __attribute__((packed)) NPCSkills {
    uint8_t barterValue;
    uint8_t bigGunsValue;
    uint8_t energyWeaponsValue;
    uint8_t explosivesValue;
    uint8_t lockpickValue;
    uint8_t medicineValue;
    uint8_t meleeWeaponsValue;
    uint8_t repairValue;
    uint8_t scienceValue;
    uint8_t gunsValue;
    uint8_t sneakValue;
    uint8_t speechValue;
    uint8_t survivalValue;
    uint8_t unarmedValue;
    uint8_t barterOffset;
    uint8_t bigGunsOffset;
    uint8_t energyWeaponsOffset;
    uint8_t explosivesOffset;
    uint8_t lockpickOffset;
    uint8_t medicineOffset;
    uint8_t meleeWeaponsOffset;
    uint8_t repairOffset;
    uint8_t scienceOffset;
    uint8_t gunsOffset;
    uint8_t sneakOffset;
    uint8_t speechOffset;
    uint8_t survivalOffset;
    uint8_t unarmedOffset;
};

struct NPC : public ScriptableRecord {
    ObjectBounds                                    objectBounds;
    std::string                                     name;
    ModelData                                       modelData;
    NPC_ACBS                                        configuration;
    std::vector<ActorFaction>                       factions;
    formid                                          deathItem; //LVLI
    formid                                          voice; //VTCK
    formid                                          templ; //NPC_ or LVLN
    formid                                          race;
    std::vector<formid>                             actorEffects; // ENCH or SPEL
    formid                                          unarmedAttackEffect;
    AttackAnimation                                 unarmedAttackAnimation;
    DestructionData                                 destructionData;
    formid                                          script;
    std::vector<std::pair<ItemData, ItemExtraData>> items;
    AIData                                          aiData;
    std::vector<formid>                             packages;
    formid                                          NPCClass;
    NPCData                                         data;
    NPCSkills                                       skills;
    std::vector<formid>                             headParts;
    formid                                          hair;
    float                                           hairLength;
    formid                                          eyes;
    uint8_t                                         hairColor[4];
    formid                                          combatStyle;
    ImpactMaterialType                              impactMaterialType;
    std::vector<uint8_t>                            FaceGenGeometrySymmetric;
    std::vector<uint8_t>                            FaceGenGeometryAsymmetric;
    std::vector<uint8_t>                            FaceGenTextureSymmetric;
    uint16_t                                        unknown;
    float                                           height;
    float                                           weight;

    virtual formid getLinkedScript() { return script; }

    NPC(ESMReader& reader);
};
}
