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
            continue;
        }
        if (reader.groupType() == ESM::GroupType::TopLevel && reader.groupLabel() == ESM::ESMType::WRLD) {
            parseWorldspaceGroup(reader);
            continue;
        }
        if (reader.groupType() == ESM::GroupType::TopLevel && reader.groupLabel() == ESM::ESMType::DIAL) {
            reader.skipGroup();
            continue;
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
    dataStores.insert(std::make_pair(ESM::ESMType::LAND, &landscapes));
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
/*
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
}*/


void GameWorld::loadCellChildren(ESM::ESMReader& reader, ESM::Cell& parentCell, uint32_t& recordsLoaded, uint32_t& recordsSkipped)
{
    //log_info("Fpointer before children 0x%x", reader.getCurrentPosition());
    assert(reader.groupType() == ESM::GroupType::CellChildren);

    uint32_t totalChildrenSkipped = 0;
    uint32_t totalChildrenLoaded  = 0;

    uint32_t cellChildrenEnd = reader.groupSize() + reader.getCurrentPosition();
    while (reader.isCurrentLocationBefore(cellChildrenEnd)) {

        reader.readNextGroupHeader();
        assert(reader.groupType() == ESM::GroupType::CellPersistentChildren || reader.groupType() == ESM::GroupType::CellTemporaryChildren || reader.groupType() == ESM::GroupType::CellVisibleDistantChildren);

        //std::unordered_map<formid, CellChildren*> childrenMap = getCellChildrenMap(reader.groupType());
        class ESM::CellChildren* children = parentCell.getChildren(reader.groupType());
        //childrenMap.insert(std::make_pair(cellId, new CellChildren));

        uint32_t skipped = 0;
        uint32_t loaded  = 0;
        while (reader.hasMoreRecordsInGroup()) {

            reader.readNextRecordHeader();
            try {
                if (reader.isCurrentRecordCompressed()) {
                    reader.startCompressedMode();
                    children->load(reader);
                    reader.endCompressedMode();
                } else {
                    children->load(reader);
                }
                loaded++;
            } catch (std::runtime_error& error) {
                log_error(error.what());
                reader.skipRecord();
                skipped++;
            }
        }
        log_info("Loaded %d children, skipped %d children of record: %d", loaded, skipped, parentCell.id);
        totalChildrenLoaded += loaded;
        totalChildrenSkipped += skipped;
    }
    recordsLoaded += totalChildrenLoaded;
    recordsSkipped += totalChildrenSkipped;
}

void GameWorld::parseCellGroup(ESM::ESMReader& reader)
{
    uint32_t cellGroupEnd   = reader.groupSize() + reader.getCurrentPosition();
    uint32_t recordsLoaded  = 0;
    uint32_t recordsSkipped = 0;
    while (reader.isCurrentLocationBefore(cellGroupEnd)) {
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

            log_info("Interior cell block: %d, subblock: %d", cellBlock, cellSubBlock);

            while (reader.isCurrentLocationBefore(interiorCellSubBlockEnd)) {
                reader.readNextRecordHeader();
                //log_info("Fpointer before cell 0x%x", reader.getCurrentPosition());
                assert(reader.recordType() == ESM::ESMType::CELL);
                formid cellId = reader.recordId();

                if (reader.isCurrentRecordCompressed()) {
                    reader.startCompressedMode();
                    this->interiorCells.load(reader);
                    reader.endCompressedMode();
                } else {
                    this->interiorCells.load(reader);
                }
                recordsLoaded++;

                this->interiorCellBlocks[cellBlock].insert(cellSubBlock, cellId);

                if (reader.peekNextType() != ESM::ESMType::GRUP) {
                    //Not every cell has children
                    continue;
                }

                ESM::Cell& loadedCell = this->interiorCells.get(cellId);
                reader.readNextGroupHeader();
                loadCellChildren(reader, loadedCell, recordsLoaded, recordsSkipped);
            }
        }
    }
    log_info("Read a total of %d records in the CELL group. Skipped a total of %d records.", recordsLoaded, recordsSkipped);
}
void GameWorld::parseWorldspaceGroup(ESM::ESMReader& reader)
{
    uint32_t worldspaceGroupEnd = reader.groupSize() + reader.getCurrentPosition();
    uint32_t recordsLoaded      = 0;
    uint32_t recordsSkipped     = 0;

    while (reader.isCurrentLocationBefore(worldspaceGroupEnd)) {

        reader.readNextRecordHeader();
        assert(reader.recordType() == ESM::ESMType::WRLD);

        formid worldspaceId = reader.recordId();
        this->worldspaces.load(reader);

        /*while (reader.peekNextType() == ESM::ESMType::WRLD) {
            reader.readNextRecordHeader();
            this->worldspaces.load(reader);
        }*/
        ESM::Worldspace& loadedWorldspace = this->worldspaces.get(worldspaceId);

        reader.readNextGroupHeader();
        assert(reader.groupType() == ESM::GroupType::WorldChildren);
        uint32_t parent = reader.groupLabel();
        log_info("Fpointer before world children 0x%x", reader.getCurrentPosition());


        uint32_t       worldChildrenEnd = reader.groupSize() + reader.getCurrentPosition();
        //WorldChildren* children         = new WorldChildren();
        //worldChildren.insert(std::make_pair(parent, children));

        if(reader.peekNextType() == ESM::ESMType::CELL) {
            reader.readNextRecordHeader();

            formid cellId = reader.recordId();
            if (reader.isCurrentRecordCompressed()) {
                reader.startCompressedMode();
                loadedWorldspace.loadCell(reader);
                reader.endCompressedMode();
            } else {
                loadedWorldspace.loadCell(reader);
            }
            recordsLoaded++;

            reader.readNextGroupHeader();
            if(reader.groupType() == ESM::GroupType::CellChildren) {
                loadCellChildren(reader, *loadedWorldspace.worldCell, recordsLoaded, recordsSkipped);
            } else {
                reader.rewind(sizeof(ESM::GroupHeader));         
            }

        }

        /*while (reader.peekNextType() == ESM::ESMType::CELL) {
            reader.readNextRecordHeader();
            //log_info("Fpointer before cell 0x%x", reader.getCurrentPosition());
            formid cellId = reader.recordId();

            if (reader.isCurrentRecordCompressed()) {
                reader.startCompressedMode();
                this->exteriorCells.load(reader);
                reader.endCompressedMode();
            } else {
                this->exteriorCells.load(reader);
            }
            recordsLoaded++;

            children->exteriorCells.insert(cellId);

            if (reader.peekNextType() != ESM::ESMType::GRUP) {
                //Not every cell has children
                continue;
            }

            loadCellChildren(reader, cellId, recordsLoaded, recordsSkipped);
        }*/

        while (reader.isCurrentLocationBefore(worldChildrenEnd)) {

            reader.readNextGroupHeader();
            assert(reader.groupType() == ESM::GroupType::ExteriorCellBlock);
            uint32_t exteriorCellBlockEnd = reader.groupSize() + reader.getCurrentPosition();

            uint32_t coords = reader.groupLabel();
            loadedWorldspace.exteriorWorldBlocks[coords];
            //children->exteriorWorldBlocks.emplace_back();
            //ExteriorWorldBlock& currentExteriorBlock = children->exteriorWorldBlocks.back();
            //currentExteriorBlock.coords              = reader.groupLabel();

            while (reader.isCurrentLocationBefore(exteriorCellBlockEnd)) {

                reader.readNextGroupHeader();
                assert(reader.groupType() == ESM::GroupType::ExteriorCellSubBlock);
                uint32_t exteriorCellSubBlockEnd = reader.groupSize() + reader.getCurrentPosition();
                uint32_t subBlockCoords = reader.groupLabel();

                //currentExteriorBlock.subblocks.emplace_back();
                //ExteriorWorldSubBlock& currentExteriorSubBlock = currentExteriorBlock.subblocks.back();
                //currentExteriorSubBlock.coords                 = reader.groupLabel();
                log_info("Exterior world block: %d, subblock: %d, location: 0x%08x", coords, subBlockCoords, reader.getCurrentPosition());
                if(reader.getCurrentPosition() == 0x019e3099) {
                    std::cout << "benis";
                }

                uint32_t subblockRecords = 0;
                while (reader.isCurrentLocationBefore(exteriorCellSubBlockEnd)) {
                    reader.readNextRecordHeader();

                    assert(reader.recordType() == ESM::ESMType::CELL);
                    formid cellId = reader.recordId();

                    if (reader.isCurrentRecordCompressed()) {
                        reader.startCompressedMode();
                        this->exteriorCells.load(reader);
                        reader.endCompressedMode();
                    } else {
                        this->exteriorCells.load(reader);
                    }
                    subblockRecords++;
                    recordsLoaded++;

                    ESM::Cell& exteriorCell = this->exteriorCells.get(cellId);
                    loadedWorldspace.exteriorWorldBlocks[coords].insert(subBlockCoords, cellId);
                    //currentExteriorSubBlock.cells.insert(cellId);

                    if (reader.peekNextType() == ESM::ESMType::GRUP) {
                        reader.readNextGroupHeader();
                        if(reader.groupType() != ESM::GroupType::CellChildren) {
                            reader.rewind(sizeof(ESM::GroupHeader));
                            continue;
                        }
                        loadCellChildren(reader, exteriorCell, recordsLoaded, recordsSkipped);
                   } 
                }
                log_info("Read %d exterior sub block cells", subblockRecords);                
            }
        }
    }
    log_info("Read a total of %d records in the CELL group. Skipped a total of %d records.", recordsLoaded, recordsSkipped);
}
};
