#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "subrecords/modeldata.hpp"
#include "structs.hpp"

namespace ESM {

class ESMReader;

enum class CardSuit : uint32_t {
    Hearts   = 1,
    Spades   = 2,
    Diamonds = 3,
    Clubs    = 4,
    Joker    = 5
};

enum class CardValues : uint32_t {
    Ace   = 1,
    Two   = 2,
    Three = 3,
    Four  = 4,
    Five  = 5,
    Six   = 6,
    Seven = 7,
    Eight = 8,
    Nine  = 9,
    Ten   = 10,
    Jack  = 12,
    Queen = 13,
    King  = 14,
    Joker = 15
};

struct CaravanCard : public Record {
    ObjectBounds objectBounds;
    std::string  name;
    ModelData    modelData;
    std::string  largeIconFilename;
    std::string  smallIconFilename;
    formid       script;
    formid       pickUpSound;
    formid       dropSound;
    std::string  highResImageFace;
    std::string  highResImageBack;
    CardSuit     cardSuit;
    CardValues   cardValue;
    uint32_t     value;

    CaravanCard(ESMReader& reader);
};
}
