#include "expl.hpp"
#include "reader.hpp"

namespace ESM {

Explosion::Explosion(ESMReader& reader)
    : Record(reader)
{
    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::EDID:
            reader.readStringSubrecord(this->editorId);
            break;
        case ESMType::FULL:
            reader.readStringSubrecord(this->name);
            break;
        case ESMType::MODL:
            ModelData::load(reader, this->modelData, 0);
            break;
        case ESMType::EITM:
            reader.readSubrecord(this->objectEffect);
            break;
        case ESMType::OBND:
            reader.readSubrecord(this->objectBounds);
            break;
        case ESMType::MNAM:
            reader.readSubrecord(this->imageSpaceModifier);
            break;
        case ESMType::INAM:
            reader.readSubrecord(this->placedImpactObject);
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
