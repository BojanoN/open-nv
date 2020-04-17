#include "hair.hpp"
#include "hdpt.hpp"
#include "reader.hpp"

namespace ESM {

Hair::Hair(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FULL);
    reader.readStringSubrecord(name);

    if (reader.peekNextType() == ESMType::MODL) {
        ModelData::load(reader, modelData, 0, ESMType::ICON);
    }

    reader.readNextSubrecordHeader();
    if (reader.getCurrentSubrecord().type == ESMType::ICON) {
        reader.checkSubrecordHeader(ESMType::ICON);
        reader.readStringSubrecord(texture);
    }
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord<uint8_t>(flags);
};
}
