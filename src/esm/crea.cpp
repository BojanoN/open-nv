#include "crea.hpp"
#include "reader.hpp"

namespace ESM {

Creature::Creature(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::OBND);
    reader.readSubrecord(objectBounds);

    if (reader.peekNextType() == ESMType::FULL) {
        reader.readNextSubrecordHeader();
        reader.readStringSubrecord(name);
    }

    if (reader.peekNextType() == ESMType::MODL) {
        reader.readNextSubrecordHeader();
        ModelData::load(reader, modelData, 0);
    }

    while (reader.peekNextType() == ESMType::SPLO) {
        actorEffects.emplace_back();
        reader.readNextSubrecordHeader();
        reader.readSubrecord(actorEffects.back());
    }

    if (reader.peekNextType() == ESMType::EITM) {
        reader.readNextSubrecordHeader();
        reader.readSubrecord(unarmedAttackEffect);
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EAMT);
    reader.readSubrecord(unarmedAttackAnimation);

    if (reader.peekNextType() == ESMType::NIFZ) {
        reader.readNextSubrecordHeader();
        reader.readStringArray(modelList);
    }

    if (reader.peekNextType() == ESMType::NIFT) {
        reader.readNextSubrecordHeader();
        reader.readArraySubrecord(textureFileHashes);
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::ACBS);
    reader.readSubrecord(configuration);

    while (reader.peekNextType() != ESMType::DATA) {
        reader.readNextSubrecordHeader();

        switch (reader.subrecordType()) {
        case (ESMType::INAM):
            reader.readSubrecord(deathItem);
            break;
        case (ESMType::VTCK):
            reader.readSubrecord(voice);
            break;
        case (ESMType::SNAM):
            factions.emplace_back();
            reader.readSubrecord(factions.back());
            break;
        case (ESMType::TPLT):
            reader.readSubrecord(templ);
            break;
        case (ESMType::AIDT):
            reader.readSubrecord(aiData);
            break;
        case (ESMType::DEST):
            DestructionData::load(reader, destructionData);
            break;
        case (ESMType::SCRI):
            reader.readSubrecord(script);
            break;
        case (ESMType::CNTO):
            items.emplace_back();
            reader.readSubrecord(items.back().first);
            break;
        case (ESMType::COED):
            reader.readSubrecord(items.back().second);
            break;
        case ESMType::PKID:
            packages.emplace_back();
            reader.readSubrecord(packages.back());
            break;
        case ESMType::KFFZ:
            reader.readStringArray(animations);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(data);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::RNAM);
    reader.readSubrecord(attackReach);

    if (reader.peekNextType() == ESMType::ZNAM) {
        reader.readNextSubrecordHeader();
        reader.readSubrecord(combatStyle);
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::PNAM);
    reader.readSubrecord(bodyPart);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::TNAM);
    reader.readSubrecord(turningSpeed);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::BNAM);
    reader.readSubrecord(baseScale);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::WNAM);
    reader.readSubrecord(footWeight);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::NAM4);
    reader.readSubrecord(impactMaterialType);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::NAM5);
    reader.readSubrecord(soundLevel);

    if (reader.peekNextType() == ESMType::CSCR) {
        reader.readNextSubrecordHeader();
        reader.readSubrecord(inheritSound);
    }

    while (reader.peekNextType() == ESMType::CSDT) {
        sounds.emplace_back();

        reader.readNextSubrecordHeader();
        reader.readSubrecord(sounds.back().type);

        reader.readNextSubrecordHeader();
        reader.checkSubrecordHeader(ESMType::CSDI);
        reader.readSubrecord(sounds.back().sound);

        reader.readNextSubrecordHeader();
        reader.checkSubrecordHeader(ESMType::CSDC);
        reader.readSubrecord(sounds.back().soundChance);
    }

    if (reader.peekNextType() == ESMType::CNAM) {
        reader.readNextSubrecordHeader();
        reader.readSubrecord(impactDataset);
    }

    if (reader.peekNextType() == ESMType::LNAM) {
        reader.readNextSubrecordHeader();
        reader.readSubrecord(meleeWeaponsList);
    }
}
}
