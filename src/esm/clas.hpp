#pragma once
#include "record.hpp"

#include <cstdint>
#include <string>

namespace ESM {

class ESMReader;

struct Attributes {
    uint8_t strength;
    uint8_t perception;
    uint8_t endurance;
    uint8_t charisma;
    uint8_t intelligence;
    uint8_t agillity;
    uint8_t luck;
};

struct ClassData {
    int32_t  tagSkill1;
    int32_t  tagSkill2;
    int32_t  tagSkill3;
    int32_t  tagSkill4;
    uint32_t flags;
    Services services;
    int8_t   teaches;
    uint8_t  maximumTrainingLevel;
    uint8_t  unused[2];
};

struct Class : public Record {

    std::string editorId;
    std::string name;
    std::string description;
    std::string largeIconFilename;
    std::string smallIconFilename;
    ClassData   classData;
    Attributes  attributes;

    Class(ESMReader& reader);
};

};
