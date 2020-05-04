#include "flst.hpp"
#include "reader.hpp"

namespace ESM {

FormIDList::FormIDList(ESMReader& reader)
    : Record(reader)
{

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::LNAM);
    reader.readArraySubrecord(this->formids);
}
}
