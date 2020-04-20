#include "gameworld.hpp"
#include "esm/utils.hpp"
#include "logc/log.h"

#include <cstring>

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
        while (reader.hasMoreRecordsInGroup()) {

            reader.readNextRecordHeader();
            GameDataBase* dataStore;
            try {
                dataStore = getDataStore(reader.recordType());
            } catch (std::runtime_error& e) {
                log_error(e.what());
                reader.skipGroup();
                break;
            }

            try {
                dataStore->load(reader);
            } catch (std::runtime_error& e) {
                log_error(e.what());
                reader.skipRecord();
            }
        }
        GameDataBase* dataStore;
        try {
            dataStore = getDataStore(reader.recordType());
            log_info("Read a total of %u records of type %s", dataStore->size(), ESM::Util::typeValueToName(reader.recordType()).c_str());
        } catch (std::runtime_error& e) {
        }
    }
}

void GameWorld::initDataStoreMap()
{
    dataStores.insert(std::make_pair(ESM::ESMType::GMST, &gameSettings));
    dataStores.insert(std::make_pair(ESM::ESMType::TXST, &textureSets));
    dataStores.insert(std::make_pair(ESM::ESMType::GLOB, &globalVariables));
    dataStores.insert(std::make_pair(ESM::ESMType::MICN, &menuIcons));
    dataStores.insert(std::make_pair(ESM::ESMType::CLAS, &classes));
    dataStores.insert(std::make_pair(ESM::ESMType::FACT, &factions));
    dataStores.insert(std::make_pair(ESM::ESMType::HDPT, &headParts));
    dataStores.insert(std::make_pair(ESM::ESMType::HAIR, &hairs));
    dataStores.insert(std::make_pair(ESM::ESMType::EYES, &eyes));
    dataStores.insert(std::make_pair(ESM::ESMType::RACE, &races));
    dataStores.insert(std::make_pair(ESM::ESMType::ENCH, &objectEffects));
    dataStores.insert(std::make_pair(ESM::ESMType::SPEL, &actorEffects));
    dataStores.insert(std::make_pair(ESM::ESMType::ACTI, &activators));
    dataStores.insert(std::make_pair(ESM::ESMType::TACT, &talkingActivators));
    dataStores.insert(std::make_pair(ESM::ESMType::TERM, &terminals));
    dataStores.insert(std::make_pair(ESM::ESMType::SOUN, &sounds));
    dataStores.insert(std::make_pair(ESM::ESMType::ASPC, &acousticSpaces));
    dataStores.insert(std::make_pair(ESM::ESMType::MGEF, &magicEffects));
    dataStores.insert(std::make_pair(ESM::ESMType::LTEX, &landscapeTextures));
    dataStores.insert(std::make_pair(ESM::ESMType::ARMO, &armors));
    dataStores.insert(std::make_pair(ESM::ESMType::SCPT, &scripts));
    dataStores.insert(std::make_pair(ESM::ESMType::BOOK, &books));
    dataStores.insert(std::make_pair(ESM::ESMType::CONT, &containers));
    dataStores.insert(std::make_pair(ESM::ESMType::DOOR, &doors));
    dataStores.insert(std::make_pair(ESM::ESMType::INGR, &ingredients));
    dataStores.insert(std::make_pair(ESM::ESMType::LIGH, &lights));
    dataStores.insert(std::make_pair(ESM::ESMType::MSTT, &moveableStatics));
    dataStores.insert(std::make_pair(ESM::ESMType::PWAT, &placeableWaters));
    dataStores.insert(std::make_pair(ESM::ESMType::TREE, &trees));
    dataStores.insert(std::make_pair(ESM::ESMType::WEAP, &weapons));
    dataStores.insert(std::make_pair(ESM::ESMType::MISC, &miscItems));
    dataStores.insert(std::make_pair(ESM::ESMType::STAT, &statics));
    dataStores.insert(std::make_pair(ESM::ESMType::SCOL, &staticCollections));
    dataStores.insert(std::make_pair(ESM::ESMType::GRAS, &grasses));
    dataStores.insert(std::make_pair(ESM::ESMType::FURN, &furnitures));
    dataStores.insert(std::make_pair(ESM::ESMType::AMMO, &ammunitions));
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
