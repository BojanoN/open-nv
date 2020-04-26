#include "note.hpp"
#include "reader.hpp"

namespace ESM {

Note::Note(ESMReader& reader)
    : Record(reader)
{

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::OBND:
            reader.readSubrecord(this->objectBounds);
            break;
        case ESMType::FULL:
            reader.readStringSubrecord(this->name);
            break;
        case ESMType::ICON:
            reader.readStringSubrecord(this->largeIconFilename);
            break;
        case ESMType::MICO:
            reader.readStringSubrecord(this->smallIconFilename);
            break;
        case ESMType::MODL:
            ModelData::load(reader, this->modelData, 0);
            break;
        case ESMType::YNAM:
            reader.readSubrecord(this->pickUpSound);
            break;
        case ESMType::ZNAM:
            reader.readSubrecord(this->dropSound);
            break;
        case ESMType::ONAM:
            reader.readSubrecord(this->quest);
            break;
        case ESMType::DATA:
            reader.readSubrecord(this->type);
            break;
        case ESMType::XNAM:
            reader.readStringSubrecord(this->texture);
            break;
        case ESMType::TNAM: {
            if (reader.getCurrentSubrecord().dataSize == sizeof(formid)) {
                this->textOrTopic.emplace<formid>();
                reader.readSubrecord(std::get<1>(this->textOrTopic));
            } else {
                this->textOrTopic.emplace<std::string>();
                reader.readStringSubrecord(std::get<0>(this->textOrTopic));
            }
            break;
        }
        case ESMType::SNAM:
            reader.readSubrecord(this->soundOrActor);
            break;

        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
