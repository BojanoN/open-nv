#include "debr.hpp"
#include "reader.hpp"

namespace ESM {

Debris::Debris(ESMReader& reader)
    : Record(reader)
{
    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::EDID:
            reader.readStringSubrecord(this->editorId);
            break;
        case ESMType::DATA: {
            this->debrisModels.emplace_back();
            uint32_t         stringSize = reader.subrecordSize() - 2;
            DebrisModelData& tmp        = debrisModels.back().modelData;
            reader.readRawData(tmp.percentage);
            tmp.modelFilename.resize(stringSize);
            reader.readRawArray(tmp.modelFilename.data(), stringSize);
            reader.readRawData(tmp.flags);
            break;
        }
        case ESMType::MODT:
            reader.readArraySubrecord(this->debrisModels.back().textureFileHashes);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
