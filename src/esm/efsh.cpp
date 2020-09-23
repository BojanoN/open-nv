#include "efsh.hpp"
#include "reader.hpp"

namespace ESM {

EffectShader::EffectShader(ESMReader& reader)
    : Record(reader)
{

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::EDID:
            reader.readStringSubrecord(this->editorId);
            break;
        case ESMType::ICON:
            reader.readStringSubrecord(this->fillTexture);
            break;
        case ESMType::ICO2:
            reader.readStringSubrecord(this->particleShaderTexture);
            break;
        case ESMType::NAM7:
            reader.readStringSubrecord(this->holesTexture);
            break;
        case ESMType::DATA:
            //reader.readSubrecord(this->data);
            reader.readRawDataSubrecSize(this->data);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
