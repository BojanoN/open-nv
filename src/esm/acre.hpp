#pragma once
#include "reader.hpp"
#include "record.hpp"
#include "subrecords/patroldata.hpp"
#include "subrecords/instance_record_common.hpp"
#include "structs.hpp"

namespace ESM {

struct PlacedCreature : public Record {

    formid base; //CREA
    formid encounterZone; //ECZN
    //ragdoll data
    //ragdoll biped data
    PatrolData                  patrolData;
    int32_t                     levelModifier;
    OwnershipData               ownershipData;
    formid                      merchantContainer; //REFR
    int32_t                     count;
    float                       radius;
    float                       health;
    std::vector<LinkedDecal>    linkedDecals;
    formid                      linkedReference;
    LinkedReferenceColor        linkedReferenceColor;
    ActivateParentsFlags        activateParentsFlags;
    std::vector<ActivateParent> activateParents;
    std::string                 activationPrompt;
    EnableParent                enableParent;
    formid                      emmitance; //LIGH, REGN
    formid                      multiBoundReference; //REFR
    bool                        ignoredBySandBox;
    float                       scale;
    PositionRotation            positionRotation;

    PlacedCreature(ESMReader& reader);
};

};
