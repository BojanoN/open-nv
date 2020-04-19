#include "door.hpp"
#include "reader.hpp"

namespace ESM {

Door::Door(ESMReader& reader)
    : Record(reader)
{
    std::unordered_set<ESMType> modelNextSubheaders = {
        ESMType::SCRI,
        ESMType::SNAM,
        ESMType::ANAM,
        ESMType::BNAM,
        ESMType::FNAM,
        ESMType::DEST
    };
    std::unordered_set<ESMType> destNextSubheaders = {
        ESMType::SNAM,
        ESMType::ANAM,
        ESMType::BNAM,
        ESMType::FNAM
    };

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
            // one fucking edge case
        case ESMType::OBND:
            reader.readSubrecord(this->objectBounds);
            break;
        case ESMType::FULL:
            reader.readStringSubrecord(this->name);
            break;
        case ESMType::MODL:
            ModelData::load(reader, this->modelData, 0);
            break;
        case ESMType::DEST:
            DestructionData::load(reader, this->destructionData);
            break;
        case ESMType::SNAM:
            reader.readSubrecord(this->openSound);
            break;
        case ESMType::ANAM:
            reader.readSubrecord(this->closeSound);
            break;
        case ESMType::BNAM:
            reader.readSubrecord(this->loopingSound);
            break;
        case ESMType::SCRI:
            reader.readSubrecord(this->script);
            break;
        case ESMType::FNAM:
            reader.readSubrecord(this->flag);
            break;

        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}

}
