#include "amef.hpp"
#include "reader.hpp"

namespace ESM {

AmmoEffect::AmmoEffect(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    if (reader.peekNextType() == ESMType::FULL) {
        reader.readNextSubrecordHeader();
        reader.readStringSubrecord(this->editorId);
    }

    if (reader.peekNextType() == ESMType::DATA) {
        reader.readNextSubrecordHeader();
        reader.readSubrecord(this->data);
    }
}
}
