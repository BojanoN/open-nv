#include "stat.hpp"
#include "reader.hpp"

namespace ESM {

Static::Static(ESMReader& reader)
    : Record(reader)
{
    std::unordered_set<ESMType> modelNextSubheaders = {
        ESMType::BRUS,
        ESMType::RNAM,
        ESMType::GRUP,
        ESMType::STAT
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
            ModelData::load(reader, this->model, 0);
            break;
        case ESMType::BRUS:
            reader.readSubrecord(this->sound);
            break;
        case ESMType::RNAM:
            reader.readSubrecord(this->loopingSound);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}

}
