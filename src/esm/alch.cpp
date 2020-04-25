#include "alch.hpp"
#include "reader.hpp"

namespace ESM {

Ingestible::Ingestible(ESMReader& reader)
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
        case ESMType::MODL:
            ModelData::load(reader, this->modelData, 0);
            break;
        case ESMType::SCRI:
            reader.readSubrecord(this->script);
            break;
        case ESMType::MICO:
            reader.readStringSubrecord(this->smallIconFilename);
            break;
        case ESMType::ICON:
            reader.readStringSubrecord(this->largeIconFilename);
            break;
        case ESMType::DATA:
            reader.readSubrecord(this->weight);
            break;
        case ESMType::ENIT:
            reader.readSubrecord(this->effectData);
            break;
        case ESMType::ETYP:
            reader.readSubrecord(this->equipmentType);
            break;
        case ESMType::EFID:
            this->effects.emplace_back();
            reader.readSubrecord(this->effects.back().baseEffect);
            break;
        case ESMType::EFIT:
            reader.readSubrecord(this->effects.back().data);
            break;
        case ESMType::CTDA:
            Condition::load(reader, this->effects.back().condition);
            break;
        case ESMType::YNAM:
            reader.readSubrecord(this->pickUpSound);
            break;
        case ESMType::ZNAM:
            reader.readSubrecord(this->dropSound);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
