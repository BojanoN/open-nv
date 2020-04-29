#include "children.hpp"

namespace ESM {

CellChildren::CellChildren() {
	typeMap.insert(std::make_pair(ESMType::REFR, &placedObjects));
	typeMap.insert(std::make_pair(ESMType::ACRE, &placedCreatures));
	typeMap.insert(std::make_pair(ESMType::PGRE, &placedGrenades));
	typeMap.insert(std::make_pair(ESMType::PMIS, &placedMissiles));
	typeMap.insert(std::make_pair(ESMType::ACHR, &placedNPCs));
	typeMap.insert(std::make_pair(ESMType::NAVM, &navigationMeshes));
}

void CellChildren::load(ESMReader& reader) {
    std::unordered_map<uint32_t, GameDataBase*>::iterator it = typeMap.find(reader.recordType());
    if (it == dataStores.end()) {
        std::stringstream s;
        s << "Wrong record type, " << ESM::Util::typeValueToName(type) << " record type not implemented!";
        throw std::runtime_error(s.str());
    }
    GameDataBase* dataStore = it->second;
    dataStore->load(reader);
}

};