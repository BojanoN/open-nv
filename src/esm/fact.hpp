#pragma once
#include "record.hpp"
#include "structs.hpp"

#include <sstream>

namespace ESM {

class ESMReader;

struct FactionData {
    uint8_t flags1;
    uint8_t flags2;
    uint8_t unused[2];
};

struct FactionRank {
    int32_t     rankNumber;
    std::string male;
    std::string female;
    uint32_t    insignia;
};

struct Faction : public Record {

    std::string                   name;
    std::vector<FactRaceRelation> relations;
    FactionData                   factionData;
    float                         unused; //?
    std::vector<FactionRank>      ranks;
    formid                        reputation;

    Faction(ESMReader& reader);

private:
    void loadRankCollection(ESMReader& reader);
};

};
