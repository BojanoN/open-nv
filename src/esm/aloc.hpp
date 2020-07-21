#pragma once
#include "record.hpp"
#include "types.hpp"

namespace ESM {

class ESMReader;

struct MediaLocationController : public Record {

    std::string name;

    float    locationDelay;
    uint32_t dayStart;
    uint32_t nightStart;
    float    retriggerDelay;

    std::vector<formid> neutralMediaSet;
    std::vector<formid> allyMediaSet;
    std::vector<formid> friendMediaSet;
    std::vector<formid> enemyMediaSet;
    std::vector<formid> locationMediaSet;
    std::vector<formid> battleMediaSet;

    formid conditionalFaction;

    MediaLocationController(ESMReader& reader);
};
}
