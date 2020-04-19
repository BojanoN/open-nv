#include "mstt.hpp"

namespace ESM {

MoveableStatic::MoveableStatic(ESMReader& reader)
    : Record(reader)
{

    std::unordered_set<ESMType> modelNextSubheaders = { ESMType::DEST, ESMType::DATA };
    std::unordered_set<ESMType> destNextSubheaders  = { ESMType::DATA };

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::OBND);
    reader.readSubrecord(objectBounds);

    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::FULL) {
        reader.readStringSubrecord(name);
        reader.readNextSubrecordHeader();
    }

    reader.checkSubrecordHeader(ESMType::MODL);
    ModelData::load(reader, modelData, 0);

    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::DEST) {
        DestructionData::load(reader, destructionData);
        reader.readNextSubrecordHeader();
    }

    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(unknown);

    if(reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        reader.checkSubrecordHeader(ESMType::SNAM);
        reader.readSubrecord(sound);
    }

}

};