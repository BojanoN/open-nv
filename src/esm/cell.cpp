#include "cell.hpp"

namespace ESM {

Cell::~Cell()
{
    delete persistentChildren;
    delete temporaryChildren;
    delete visibleDistantChildren;
}

CellChildren::CellChildren()
{
    typeMap.insert(std::make_pair(ESM::ESMType::REFR, &placedObjects));
    typeMap.insert(std::make_pair(ESM::ESMType::ACRE, &placedCreatures));
    typeMap.insert(std::make_pair(ESM::ESMType::PGRE, &placedGrenades));
    //typeMap.insert(std::make_pair(ESM::ESMType::PMIS, &placedMissiles));
    typeMap.insert(std::make_pair(ESM::ESMType::ACHR, &placedNPCs));
    typeMap.insert(std::make_pair(ESM::ESMType::NAVM, &navigationMeshes));
    typeMap.insert(std::make_pair(ESM::ESMType::LAND, &landscapes));
}

Record* CellChildren::load(ESM::ESMReader& reader)
{
    std::unordered_map<uint32_t, GameWorld::GameDataBase*>::iterator it = typeMap.find(reader.recordType());
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
    Record*                  ret       = dataStore->load(reader);
#ifdef DEBUG
    log_debug("Child with id: %d, of type %s", id, ESM::Util::typeValueToName(type).c_str());
#endif
    return ret;
}

formid CellChildren::getByEditorID(std::string& editorId)
{
    formid ret;

    ret = placedNPCs.get(editorId);
    if (ret) {
        return ret;
    }

    ret = placedCreatures.get(editorId);
    if (ret) {
        return ret;
    }
    ret = placedObjects.get(editorId);
    if (ret) {
        return ret;
    }

    return 0;
}

formid Cell::getByEditorID(std::string& editorId)
{
    formid ret;

    ret = persistentChildren->getByEditorID(editorId);
    if (ret) {
        return ret;
    }

    ret = temporaryChildren->getByEditorID(editorId);
    if (ret) {
        return ret;
    }
    ret = visibleDistantChildren->getByEditorID(editorId);
    if (ret) {
        return ret;
    }
    return ret;
}

class CellChildren* Cell::getChildren(uint32_t groupType)
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

Cell::Cell(ESMReader& reader)
    : Record(reader)
{
    persistentChildren     = new class CellChildren;
    temporaryChildren      = new class CellChildren;
    visibleDistantChildren = new class CellChildren;

    reader.readNextSubrecordHeader();
    if (reader.subrecordType() == ESMType::EDID) {
        reader.checkSubrecordHeader(ESMType::EDID);
        reader.readStringSubrecord(editorId);
        reader.readNextSubrecordHeader();
    }

    if (reader.subrecordType() == ESMType::FULL) {
        reader.readStringSubrecord(name);
        reader.readNextSubrecordHeader();
    }

    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(flags);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::XCLC:
            reader.readSubrecord(grid);
            break;
        case ESMType::XCLL:
            reader.readSubrecord(lighting);
            break;
        case ESMType::IMPF:
            reader.readSubrecord(footstepMaterial);
            break;
        case ESMType::LTMP:
            reader.readSubrecord(lightTemplate.templ);
            break;
        case ESMType::LNAM:
            reader.readSubrecord(lightTemplate.inheritFlags);
            break;
        case ESMType::XCLW:
            reader.readSubrecord(waterHeight);
            break;
        case ESMType::XNAM:
            reader.readStringSubrecord(waterNoiseTexture);
            break;
        case ESMType::XCLR:
            reader.readArraySubrecord(regions);
            break;
        case ESMType::XCIM:
            reader.readSubrecord(imageSpace);
            break;
        case ESMType::XCET:
            reader.readSubrecord(unknown);
            break;
        case ESMType::XEZN:
            reader.readSubrecord(encounterZone);
            break;
        case ESMType::XCCM:
            reader.readSubrecord(climate);
            break;
        case ESMType::XCWT:
            reader.readSubrecord(water);
            break;
        case ESMType::XOWN:
            reader.readSubrecord(owner);
            break;
        case ESMType::XRNK:
            reader.readSubrecord(factionRank);
            break;
        case ESMType::XCAS:
            reader.readSubrecord(acousticSpace);
            break;
        case ESMType::XCMT:
            reader.readSubrecord(unused);
            break;
        case ESMType::XCMO:
            reader.readSubrecord(musicType);
            break;
        default:
            std::stringstream s;
            log_fatal("Invalid subrecord type %s", Util::typeValueToName(reader.subrecordType()).c_str());
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}

};
