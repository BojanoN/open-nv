#include "fnv.hpp"

namespace Game {

const std::unordered_set<ESM::ESMType> FalloutNVGame::initialToplevelGroups {
    ESM::ESMType::PROJ,
    ESM::ESMType::IDLM,
    ESM::ESMType::LVLN,
    ESM::ESMType::LVLC,
    ESM::ESMType::NPC_,
    ESM::ESMType::LVLI,
    //    ESM::ESMType::WEAP,
    ESM::ESMType::FURN,
    ESM::ESMType::TREE,
    ESM::ESMType::GRAS,
    ESM::ESMType::PWAT,
    ESM::ESMType::ASPC,
    ESM::ESMType::RACE,
    // ESM::ESMType::NAVI,
    ESM::ESMType::GLOB,
    ESM::ESMType::REGN,
    ESM::ESMType::SPEL,
    ESM::ESMType::EYES,
    ESM::ESMType::WTHR,
    ESM::ESMType::KEYM,
    ESM::ESMType::SOUN,
    ESM::ESMType::CLMT,
    ESM::ESMType::MSTT,
    ESM::ESMType::LTEX,
    ESM::ESMType::HDPT,
    ESM::ESMType::GMST,
    ESM::ESMType::CREA,
    ESM::ESMType::FACT,
    ESM::ESMType::AMMO,
    ESM::ESMType::LIGH,
    ESM::ESMType::ALCH,
    ESM::ESMType::HAIR,
    ESM::ESMType::CLAS,
    ESM::ESMType::BOOK,
    ESM::ESMType::SCPT,
    ESM::ESMType::MGEF,
    ESM::ESMType::STAT,
    ESM::ESMType::ACTI,
    ESM::ESMType::NOTE,
    ESM::ESMType::TACT,
    ESM::ESMType::DOOR,
    ESM::ESMType::TERM,
    ESM::ESMType::MICN,
    ESM::ESMType::ENCH,
    ESM::ESMType::CONT,
    ESM::ESMType::INGR,
    ESM::ESMType::MISC,
    ESM::ESMType::ARMO,
    ESM::ESMType::TXST,
    ESM::ESMType::SCOL,
};

void FalloutNVGame::start()
{
    world.loadTopLevelGroups("FalloutNV.esm", FalloutNVGame::initialToplevelGroups);
}
}
