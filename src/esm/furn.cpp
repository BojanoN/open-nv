#include "furn.hpp"
#include "reader.hpp"

namespace ESM {

Furniture::Furniture(ESMReader& reader)
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
        case ESMType::DEST:
            DestructionData::load(reader, this->destructionData);
            break;
        case ESMType::MNAM:
            reader.readArraySubrecord(this->markerFlags);
            break;
        case ESMType::SCRI:
            reader.readSubrecord(this->script);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
