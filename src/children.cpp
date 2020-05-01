#include "children.hpp"

namespace GameWorld {

CellChildren::CellChildren()
{
    typeMap.insert(std::make_pair(ESM::ESMType::REFR, &placedObjects));
    typeMap.insert(std::make_pair(ESM::ESMType::ACRE, &placedCreatures));
    //typeMap.insert(std::make_pair(ESM::ESMType::PGRE, &placedGrenades));
    //typeMap.insert(std::make_pair(ESM::ESMType::PMIS, &placedMissiles));
    typeMap.insert(std::make_pair(ESM::ESMType::ACHR, &placedNPCs));
    typeMap.insert(std::make_pair(ESM::ESMType::NAVM, &navigationMeshes));
}

void CellChildren::load(ESM::ESMReader& reader)
{
    std::unordered_map<uint32_t, GameDataBase*>::iterator it = typeMap.find(reader.recordType());
    if (it == this->typeMap.end()) {
        std::stringstream s;
        s << "Wrong record type, " << ESM::Util::typeValueToName(reader.recordType()) << " record type not implemented!";
        throw std::runtime_error(s.str());
    }
#ifdef DEBUG
    formid   id   = reader.recordId();
    uint32_t type = reader.recordType();
#endif
    GameWorld::GameDataBase* dataStore = it->second;
    dataStore->load(reader);
#ifdef DEBUG
    log_debug("Child with id: %d, of type %s", id, ESM::Util::typeValueToName(type).c_str());
#endif
}

};
