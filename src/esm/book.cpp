#include "book.hpp"
#include "reader.hpp"

namespace ESM {
Book::Book(ESMReader& reader)
    : Record(reader)
{

    std::unordered_set<ESMType> modelNextSubheaders = { ESMType::ICON, ESMType::MICO, ESMType::SCRI, ESMType::DESC };
    std::unordered_set<ESMType> destNextSubheaders  = { ESMType::YNAM, ESMType::ZNAM, ESMType::DATA };

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::OBND);
    reader.readSubrecord(this->objectBounds);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::FULL:
            reader.readStringSubrecord(this->name);
            break;
        case ESMType::MODL:
            ModelData::load(reader, this->modelData, 0);
            break;
        case ESMType::ICON:
            reader.readStringSubrecord(this->largeIconFilename);
            break;
        case ESMType::MICO:
            reader.readStringSubrecord(this->smallIconFilename);
            break;
        case ESMType::DESC:
            reader.readStringSubrecord(this->description);
            break;
        case ESMType::DEST:
            DestructionData::load(reader, this->destructionData);
            break;
        case ESMType::ZNAM:
            reader.readSubrecord(this->pickUpSound);
            break;
        case ESMType::YNAM:
            reader.readSubrecord(this->dropSound);
            break;
        case ESMType::SCRI:
            reader.readSubrecord(this->script);
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
