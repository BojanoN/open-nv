#include "cont.hpp"
#include "reader.hpp"

namespace ESM {

Container::Container(ESMReader& reader)
    : Record(reader)
{
    std::unordered_set<ESMType> modelNextSubheaders = { ESMType::SCRI, ESMType::CNTO, ESMType::DATA, ESMType::DEST };
    std::unordered_set<ESMType> destNextSubheaders  = { ESMType::SNAM, ESMType::QNAM, ESMType::RNAM, ESMType::DATA };

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::OBND);
    reader.readSubrecord(this->objectBounds);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::CNTO:
            this->items.emplace_back();
            reader.readSubrecord(this->items.back().first);
            break;
        case ESMType::COED:
            reader.readSubrecord(this->items.back().second);
            break;
        case ESMType::FULL:
            reader.readStringSubrecord(this->name);
            break;
        case ESMType::MODL:
            ModelData::load(reader, this->modelData, 0, modelNextSubheaders);
            break;
        case ESMType::DEST:
            DestructionData::load(reader, this->destructionData, destNextSubheaders);
            break;
        case ESMType::SNAM:
            reader.readSubrecord(this->openSound);
            break;
        case ESMType::QNAM:
            reader.readSubrecord(this->closeSound);
            break;
        case ESMType::RNAM:
            reader.readSubrecord(this->loopingSound);
            break;
        case ESMType::SCRI:
            reader.readSubrecord(this->script);
            break;
        case ESMType::DATA:
            reader.readSubrecord(this->containerData);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
