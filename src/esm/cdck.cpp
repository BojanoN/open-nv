#include "cdck.hpp"
#include "reader.hpp"

namespace ESM {

CaravanDeck::CaravanDeck(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::FULL:
            reader.readStringSubrecord(this->name);
            break;
        case ESMType::CARD:
            reader.readArraySubrecord(this->cards);
            break;
        case ESMType::DATA:
            reader.readSubrecord(this->data);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
