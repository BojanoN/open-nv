#include "clmt.hpp"
#include "reader.hpp"

namespace ESM {

Climate::Climate(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();

        switch (reader.subrecordType()) {
        case ESMType::WLST:
            reader.readArraySubrecord(this->weatherTypes);
            break;
        case ESMType::FNAM:
            reader.readStringSubrecord(this->sunTexture);
            break;
        case ESMType::GNAM:
            reader.readStringSubrecord(this->sunGlareTexture);
            break;
        case ESMType::MODL:
            ModelData::load(reader, this->modelData, 0);
            break;
        case ESMType::TNAM:
            reader.readSubrecord(this->timing);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
