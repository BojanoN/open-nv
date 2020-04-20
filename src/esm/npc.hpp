#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

class ESMReader;

struct NPC : public Record {
    std::string                                 editorId;
    ObjectBounds                                objectBounds;
    std::string                                 name;
    ModelData                                   modelData;
    NPC_ACBS                                    configuration;
    formid                                      deathItem; //LVLI
    formid                                      voice; //VTCK
    formid                                      templ; //NPC_ or LVLN
    formid                                      race;
    formid                                      actorEffect; // ENCH or SPEL
    formid                                      unarmedAttackEffect;
    AttackAnimations                            unarmedAttackAnimation;
    DestructionData                             destructionData;
    formid                                      script;
    std::vector<std::pair<Item, ItemExtraData>> items;
    AIDT                                        aiData;
};
}
