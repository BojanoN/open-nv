#include "hdpt.hpp"

namespace ESM {

HeadPart::HeadPart(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FULL);
    reader.readStringSubrecord(this->name);

    reader.readNextSubrecordHeader();
    if (reader.subrecordType() == ESMType::MODL) {
        //reader.skipSubrecord();
        std::unordered_set<ESMType> next { ESMType::DATA };
        ModelData::load(reader, modelData, 0);
        reader.readNextSubrecordHeader();
    }

    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(flags);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        reader.checkSubrecordHeader(ESMType::HNAM);
        extraParts.push_back(0);
        reader.readSubrecord(extraParts.back());
    }
};
}
