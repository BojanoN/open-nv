#include "npc.hpp"
#include "reader.hpp"

namespace ESM {

NPC::NPC(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::OBND);
    reader.readSubrecord(objectBounds);

    while (reader.peekNextType() != ESMType::ACBS) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::FULL:
            reader.readStringSubrecord(name);
            break;
        case ESMType::MODL:
            ModelData::load(reader, modelData, 0);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::ACBS);
    reader.readSubrecord(configuration);

    reader.readNextSubrecordHeader();
    while (reader.subrecordType() == ESMType::SNAM) {
        factions.emplace_back();
        reader.readSubrecord(factions.back());
        reader.readNextSubrecordHeader();
    }
    if (reader.subrecordType() == ESMType::INAM) {
        reader.readSubrecord(deathItem);
        reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::VTCK);
    reader.readSubrecord(voice);

    reader.readNextSubrecordHeader();
    if (reader.subrecordType() == ESMType::TPLT) {
        reader.readSubrecord(templ);
        reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::RNAM);
    reader.readSubrecord(race);

    reader.readNextSubrecordHeader();
    while (reader.subrecordType() == ESMType::SPLO) {
        actorEffects.emplace_back();
        reader.readSubrecord(actorEffects.back());
        reader.readNextSubrecordHeader();
    }
    if (reader.subrecordType() == ESMType::EITM) {
        reader.readSubrecord(unarmedAttackEffect);
        reader.readNextSubrecordHeader();
    }

    reader.checkSubrecordHeader(ESMType::EAMT);
    reader.readSubrecord(unarmedAttackAnimation);

    while (reader.peekNextType() == ESMType::CNAM) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::DEST:
            DestructionData::load(reader, destructionData);
            break;
        case ESMType::SCRI:
            reader.readSubrecord(script);
            break;
        case ESMType::CNTO:
            items.emplace_back();
            reader.readSubrecord(items.back().first);
            reader.readNextSubrecordHeader();
            reader.checkSubrecordHeader(ESMType::COED);
            reader.readSubrecord(items.back().second);
            break;
        case ESMType::AIDT:
            reader.readSubrecord(aiData);
            break;
        case ESMType::PKID:
            packages.emplace_back();
            reader.readSubrecord(packages.back());
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::CNAM);
    reader.readSubrecord(NPCClass);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(data);

    while (reader.peekNextType() == ESMType::HCLR) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::DNAM:
            reader.readSubrecord(skills);
            break;
        case ESMType::PNAM:
            headParts.emplace_back();
            reader.readSubrecord(headParts.back());
            break;
        case ESMType::HNAM:
            reader.readSubrecord(hair);
            break;
        case ESMType::LNAM:
            reader.readSubrecord(hairLength);
            break;
        case ESMType::ENAM:
            reader.readSubrecord(eyes);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::HCLR);
    reader.readFixedArraySubrecord(hairColor);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::ZNAM:
            reader.readSubrecord(combatStyle);
            break;
        case ESMType::NAM4:
            reader.readSubrecord(impactMaterialType);
            break;
        case ESMType::FGGS:
            reader.readArraySubrecord(FaceGenGeometrySymmetric);
            break;
        case ESMType::FGGA:
            reader.readArraySubrecord(FaceGenGeometryAsymmetric);
            break;
        case ESMType::FGTS:
            reader.readArraySubrecord(FaceGenTextureSymmetric);
            break;
        case ESMType::NAM5:
            reader.readSubrecord(unknown);
            break;
        case ESMType::NAM6:
            reader.readSubrecord(height);
            break;
        case ESMType::NAM7:
            reader.readSubrecord(weight);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}

}
