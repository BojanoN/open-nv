#pragma once
#include "record.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace ESM {

class ESMReader;

struct SlotReelStop {
    uint32_t symbol1;
    uint32_t symbol2;
    uint32_t symbol3;
    uint32_t symbol4;
    uint32_t symbol5;
    uint32_t symbol6;
    uint32_t symbolW;
};

enum class CasinoFlags : uint32_t {
    DealerStaySoft17 = 1
};

struct CasinoData {
    float        decksPercentBeforeShuffle;
    float        blackjackPayoutRatio;
    SlotReelStop slotData;
    uint32_t     numberOfDecks;
    uint32_t     maxWinnings;
    formid       currency;
    formid       casinoWinningsQuest;
    CasinoFlags  flags;
};

struct CasinoChipModel {
    std::string OneDollarChip;
    std::string FiveDollarChip;
    std::string TenDollarChip;
    std::string TwentyFiveDollarChip;
    std::string HundredDollarChip;
    std::string FiveHundredDollarChip;
    std::string rouletteChipModel;
    std::string slotMachineModel;
};

struct SlotReelTexture {
    std::string symbol1;
    std::string symbol2;
    std::string symbol3;
    std::string symbol4;
    std::string symbol5;
    std::string symbol6;
    std::string symbolW;
};

struct BlackjackTexture {
    std::string deck1;
    std::string deck2;
    std::string deck3;
    std::string deck4;
};

struct Casino : public Record {
    std::string      editorId;
    std::string      name;
    CasinoData       data;
    CasinoChipModel  chipModel;
    std::string      blackjackTableModel;
    std::string      rouletteTableModel;
    SlotReelTexture  slotReelTexture;
    BlackjackTexture blackjackTexture;

    Casino(ESMReader& reader);
};
}
