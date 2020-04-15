#include "gameworld.hpp"

namespace GameWorld {

void GameWorld::load(ESM::ESMReader& reader) {

	if(!reader.hasMoreBytes()) {
		std::stringstream s;
		s << "File " << reader.getFileName() << " is empty!\n";
		throw std::runtime_error(s.str())
	}

	reader.readNextRecordHeader();
	if(reader.getCurrentRecordType().intValue != ESM::RecordType.TES4) {
		std::stringstream s;
		s << "File " << reader.getFileName() << " is not a valid plugin file!\n";
		throw std::runtime_error(s.str())
	}

	skipRecord();
	
	while(reader.hasMoreBytes()) {
		reader.readNextGroupHeader();
		while(reader.hasMoreRecordsInGroup()) {

			reader.readNextRecordHeader();
			auto dataStore = getDataStoreForType(reader.getCurrentRecordType());
			dataStore.load(reader);
		}
	}

}

};
