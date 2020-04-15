#include "gameworld.hpp"
#include "esm/utils.hpp"

namespace GameWorld {

void GameWorld::load(ESM::ESMReader& reader) {

	if(!reader.hasMoreBytes()) {
		std::stringstream s;
		s << "File " << reader.getFileName() << " is empty!\n";
		throw std::runtime_error(s.str());
	}

	reader.readNextRecordHeader();
	if(reader.recordType() != ESM::ESMType::TES4) {
		std::stringstream s;
		s << "File " << reader.getFileName() << " is not a valid plugin file!\n";
		throw std::runtime_error(s.str());
	}

	reader.skipRecord();
	
	while(reader.hasMoreBytes()) {
		reader.readNextGroupHeader();
		while(reader.hasMoreRecordsInGroup()) {

			reader.readNextRecordHeader();
			GameDataBase* dataStore;
			try {
				dataStore = getDataStore(reader.recordType());
			} catch (std::runtime_error e) {
				std::cerr << e.what();
				reader.skipGroup();
				break;
			}

			try {
				dataStore->load(reader);
			} catch (std::runtime_error e) {
				std::cerr << e.what();
				reader.skipRecord();
			}
			
		}
	}

}

void GameWorld::initDataStoreMap() {
  dataStores.insert(std::pair<uint32_t, GameDataBase*>(ESM::ESMType::TXST, &textureSets));
}

GameDataBase* GameWorld::getDataStore(uint32_t type) {
	std::unordered_map<uint32_t, GameDataBase*>::iterator it = dataStores.find(type);
	if(it == dataStores.end()) {
		std::stringstream s;
      s << ESM::Util::typeValueToName(type) << " record type not implemented!\n";
		throw std::runtime_error(s.str());
	}
	return it->second;
}

};
