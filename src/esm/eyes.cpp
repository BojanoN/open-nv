#include "eyes.hpp"
#include "reader.hpp"
#include "types.hpp"

namespace ESM {

Eyes::Eyes(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FULL);
    reader.readStringSubrecord(name);

    if (reader.peekNextType() == ESMType::ICON) {
        reader.readNextSubrecordHeader();
        reader.readStringSubrecord(texture);
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(flags);
};
}
