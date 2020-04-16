#include "hdpt.hpp"

namespace ESM {

HeadPart::HeadPart(ESMReader& reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FULL);
    reader.readStringSubrecord(this->name);

    if (reader.peekNextType() == ESMType::MODL) {
        ModelData::load(reader, modelData, 0, ESMType::DATA);
    }

    reader.readNextSubrecordHeader();
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
