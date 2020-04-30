#include "gameworld.hpp"
#include "esm/headers.hpp"
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

        /*if (reader.groupType() != ESM::GroupType::TopLevel) {
            reader.skipGroup();
            log_debug("Skipping non-toplevel group..");
        }*/
        if (reader.groupType() == ESM::GroupType::TopLevel && reader.groupLabel() == ESM::ESMType::CELL) {
            parseCellGroup(reader);
        }

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
                if (reader.isCurrentRecordCompressed()) {
                    reader.startCompressedMode();
                    dataStore->load(reader);
                    reader.endCompressedMode();
                } else {
                    dataStore->load(reader);
                }
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
    //dataStores.insert(std::make_pair(ESM::ESMType::WEAP, &weapons));
    dataStores.insert(std::make_pair(ESM::ESMType::MISC, &miscItems));
    dataStores.insert(std::make_pair(ESM::ESMType::STAT, &statics));
    dataStores.insert(std::make_pair(ESM::ESMType::SCOL, &staticCollections));
    dataStores.insert(std::make_pair(ESM::ESMType::GRAS, &grasses));
    dataStores.insert(std::make_pair(ESM::ESMType::FURN, &furnitures));
    dataStores.insert(std::make_pair(ESM::ESMType::AMMO, &ammunitions));
    dataStores.insert(std::make_pair(ESM::ESMType::NPC_, &NPCs));
    dataStores.insert(std::make_pair(ESM::ESMType::CREA, &creatures));
    dataStores.insert(std::make_pair(ESM::ESMType::LVLC, &leveledCreatures));
    dataStores.insert(std::make_pair(ESM::ESMType::LVLN, &leveledNPCs));
    dataStores.insert(std::make_pair(ESM::ESMType::KEYM, &keys));
    dataStores.insert(std::make_pair(ESM::ESMType::ALCH, &ingestibles));
    dataStores.insert(std::make_pair(ESM::ESMType::IDLM, &idleMarkers));
    dataStores.insert(std::make_pair(ESM::ESMType::NOTE, &notes));
    dataStores.insert(std::make_pair(ESM::ESMType::PROJ, &projectiles));
    dataStores.insert(std::make_pair(ESM::ESMType::LVLI, &leveledItems));
    dataStores.insert(std::make_pair(ESM::ESMType::WTHR, &weather));
    dataStores.insert(std::make_pair(ESM::ESMType::CLMT, &climates));
    dataStores.insert(std::make_pair(ESM::ESMType::REGN, &regions));
    dataStores.insert(std::make_pair(ESM::ESMType::WRLD, &worldspaces));
    dataStores.insert(std::make_pair(ESM::ESMType::DIAL, &dialogueTopics));
}

GameDataBase* GameWorld::getDataStore(uint32_t type)
{
    std::unordered_map<uint32_t, GameDataBase*>::iterator it = dataStores.find(type);
    if (it == dataStores.end()) {
        std::stringstream s;
        s << "Wrong record type, " << ESM::Util::typeValueToName(type) << " record type not implemented!";
        throw std::runtime_error(s.str());
    }
    return it->second;
}

std::unordered_map<formid, CellChildren*>& GameWorld::getCellChildrenMap(uint32_t groupType)
{
    switch (groupType) {
    case ESM::GroupType::CellPersistentChildren:
        return persistentChildren;
    case ESM::GroupType::CellTemporaryChildren:
        return temporaryChildren;
    case ESM::GroupType::CellVisibleDistantChildren:
        return visibleDistantChildren;
    default:
        std::stringstream s;
        s << groupType << " is not a valid cell children group type!";
        throw std::runtime_error(s.str());
    }
}

void GameWorld::parseCellGroup(ESM::ESMReader& reader)
{
    while (reader.hasMoreRecordsInGroup()) {
        reader.readNextGroupHeader();

        assert(reader.groupType() == ESM::GroupType::InteriorCellBlock);
        uint32_t interiorCellBlockEnd = reader.groupSize() + reader.getCurrentPosition();
        uint32_t cellBlock            = reader.groupLabel();
        this->interiorCellBlocks[cellBlock];

        while (reader.isCurrentLocationBefore(interiorCellBlockEnd)) {
            reader.readNextGroupHeader();

            assert(reader.groupType() == ESM::GroupType::InteriorCellSubBlock);
            uint32_t interiorCellSubBlockEnd = reader.groupSize() + reader.getCurrentPosition();
            uint32_t cellSubBlock            = reader.groupLabel();

            while (reader.isCurrentLocationBefore(interiorCellSubBlockEnd)) {
                reader.readNextRecordHeader();
                assert(reader.recordType() == ESM::ESMType::CELL);
                formid cellId = reader.recordId();

                if (reader.isCurrentRecordCompressed()) {
                    reader.startCompressedMode();
                    this->interiorCells.load(reader);
                    reader.endCompressedMode();
                } else {
                    this->interiorCells.load(reader);
                }

                this->interiorCellBlocks[cellBlock].insert(cellSubBlock, cellId);

                reader.readNextGroupHeader();
                assert(reader.groupType() == ESM::GroupType::CellChildren);

                uint32_t cellChildrenEnd = reader.groupSize() + reader.getCurrentPosition();
                while (reader.isCurrentLocationBefore(cellChildrenEnd)) {

                    reader.readNextGroupHeader();
                    assert(reader.groupType() == ESM::GroupType::CellPersistentChildren || reader.groupType() == ESM::GroupType::CellTemporaryChildren || reader.groupType() == ESM::GroupType::CellVisibleDistantChildren);

                    std::unordered_map<formid, CellChildren*> childrenMap = getCellChildrenMap(reader.groupType());
                    childrenMap.insert(std::make_pair(cellId, new CellChildren));

                    while (reader.hasMoreRecordsInGroup()) {

                        reader.readNextRecordHeader();
                        if (reader.isCurrentRecordCompressed()) {
                            reader.startCompressedMode();
                            childrenMap[cellId]->load(reader);
                            reader.endCompressedMode();
                        } else {
                            childrenMap[cellId]->load(reader);
                        }
                    }
                }
            }
        }
    }
}

};
