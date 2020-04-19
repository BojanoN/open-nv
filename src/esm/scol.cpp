#include "scol.hpp"
#include "reader.hpp"

namespace ESM {
StaticCollection::StaticCollection(ESMReader& reader)
    : Record(reader)
{
    std::unordered_set<ESMType> modelNextSubheaders = {
        ESMType::MODL,
        ESMType::ONAM,
        ESMType::DATA,
        ESMType::GRUP,
        ESMType::SCOL
    };

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::OBND:
            reader.readSubrecord(this->objectBounds);
            break;
        case ESMType::MODL:
            ModelData::load(reader, this->modelData, 0, modelNextSubheaders);
            break;
        case ESMType::ONAM:
            this->parts.emplace_back();
            reader.readSubrecord(this->parts.back().stat);
            break;
        case ESMType::DATA:
            reader.readArraySubrecord(this->parts.back().placements);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
