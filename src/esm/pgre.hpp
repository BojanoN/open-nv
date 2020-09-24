#pragma once
#include "record.hpp"
#include "subrecords/patroldata.hpp"
#include "subrecords/instance_record_common.hpp"
#include "structs.hpp"

namespace ESM {

class ESMReader;

struct PlacedGrenade : public Record {
    formid      base; //CREA
    formid      encounterZone; //ECZN
    //ragdoll data
    //ragdoll biped data
    PatrolData                   patrolData;
    OwnershipData                ownershipData;
    formid                       merchantContainer; //REFR
    int32_t                      count;
    float                        radius;
    float                        health;
    std::vector<WaterRenderData> waterReflection;

    std::vector<LinkedDecal>    linkedDecals;
    formid                      linkedReference;
    LinkedReferenceColor        linkedReferenceColor;
    ActivateParentsFlags        activateParentsFlags;
    std::vector<ActivateParent> activateParents;
    std::string                 activationPrompt;
    EnableParent                enableParent;
    formid                      emittance; //LIGH, REGN
    formid                      multiBoundReference; //REFR
    bool                        ignoredBySandBox;
    float                       scale;
    PositionRotation            positionRotation;

    PlacedGrenade(ESMReader& reader);
};

}
