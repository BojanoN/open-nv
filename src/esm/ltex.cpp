#include "ltex.hpp"
#include "reader.hpp"

namespace ESM {

LandscapeTexture::LandscapeTexture(ESMReader& reader)
    : Record(reader)
{
    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();

        switch (reader.subrecordType()) {
        case (ESMType::EDID):
            reader.readStringSubrecord(this->editorId);
            break;
        case (ESMType::ICON):
            reader.readStringSubrecord(this->largeIconFilename);
            break;
        case (ESMType::MICO):
            reader.readStringSubrecord(this->smallIconFilename);
            break;
        case (ESMType::GNAM):
            this->grass.emplace_back();
            reader.readSubrecord(grass.back());
            break;
        case (ESMType::SNAM):
            reader.readSubrecord(this->textureSpecularExponent);
            break;
        case (ESMType::TNAM):
            reader.readSubrecord(this->texture);
            break;
        case (ESMType::HNAM):
            reader.readSubrecord(this->havok);
            break;
        default:
            reader.reportError("Unknown subrecord!");
            break;
        }
    }
}

}
