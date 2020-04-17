#include "gameworld.hpp"
#include "esm/utils.hpp"
#include "logc/log.h"

namespace GameWorld {

void GameWorld::load(ESM::ESMReader& reader)
{

    if (!reader.hasMoreBytes()) {
        std::stringstream s;
        s << "File " << reader.getFileName() << " is empty!\n";
        throw std::runtime_error(s.str());
    }

    reader.readNextRecordHeader();
    if (reader.recordType() != ESM::ESMType::TES4) {
        std::stringstream s;
        s << "File " << reader.getFileName() << " is not a valid plugin file!\n";
        throw std::runtime_error(s.str());
    }

    reader.skipRecord();

    while (reader.hasMoreBytes()) {
        reader.readNextGroupHeader();
        int counter = 0;
        while (reader.hasMoreRecordsInGroup()) {

            reader.readNextRecordHeader();
            GameDataBase* dataStore;
            try {
                dataStore = getDataStore(reader.recordType());
            } catch (std::runtime_error e) {
                log_error(e.what());
                reader.skipGroup();
                break;
            }

            try {
                dataStore->load(reader);
                counter++;
            } catch (std::runtime_error e) {
                log_error(e.what());

                reader.skipRecord();
            }
        }
        GameDataBase* dataStore;
        try {
            dataStore = getDataStore(reader.recordType());
            log_info("Read a total of %u records of type %s", dataStore->size(), ESM::Util::typeValueToName(reader.recordType()).c_str());
        } catch (std::runtime_error e) {
        }
    }
}

void GameWorld::initDataStoreMap()
{
    dataStores.insert(std::pair<uint32_t, GameDataBase*>(ESM::ESMType::TXST, &textureSets));
    dataStores.insert(std::pair<uint32_t, GameDataBase*>(ESM::ESMType::MICN, &menuIcons));
    dataStores.insert(std::pair<uint32_t, GameDataBase*>(ESM::ESMType::CLAS, &classes));
    dataStores.insert(std::pair<uint32_t, GameDataBase*>(ESM::ESMType::FACT, &factions));
    dataStores.insert(std::pair<uint32_t, GameDataBase*>(ESM::ESMType::HDPT, &headParts));
    dataStores.insert(std::pair<uint32_t, GameDataBase*>(ESM::ESMType::HAIR, &hairs));
}

GameDataBase* GameWorld::getDataStore(uint32_t type)
{
    std::unordered_map<uint32_t, GameDataBase*>::iterator it = dataStores.find(type);
    if (it == dataStores.end()) {
        std::stringstream s;
        s << ESM::Util::typeValueToName(type) << " record type not implemented!";
        throw std::runtime_error(s.str());
    }
    return it->second;
}

};
