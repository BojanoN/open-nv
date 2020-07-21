#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

class ESMReader;

struct CaravanDeck : public Record {
    std::string name;

    std::vector<formid> cards;
    uint32_t            data;

    CaravanDeck(ESMReader& reader);
};
}
