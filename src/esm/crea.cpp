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

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::MODL);
    ModelData::load(reader, modelData, 0);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::SPLO);
    reader.readSubrecord(actorEffect);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EITM);
    reader.readSubrecord(unarmedAttackEffect);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EAMT);
    reader.readSubrecord(unarmedAttackAnimation);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EAMT);
    reader.readStringArray(modelList);
}
}
