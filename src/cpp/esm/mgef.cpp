#include "mgef.hpp"
#include "reader.hpp"

namespace ESM {

MagicEffect::MagicEffect(ESMReader& reader)
    : Record(reader)
{
    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();

        switch (reader.subrecordType()) {
        case (ESMType::EDID):
            reader.readStringSubrecord(this->editorId);
            break;
        case (ESMType::FULL):
            reader.readStringSubrecord(this->name);
            break;
        case (ESMType::DESC):
            reader.readStringSubrecord(this->description);
            break;
        case (ESMType::ICON):
            reader.readStringSubrecord(this->largeIconFilename);
            break;
        case (ESMType::MICO):
            reader.readStringSubrecord(this->smallIconFilename);
            break;
        case (ESMType::MODL):
            this->modelData.emplace_back();
            ModelData::loadCollection(reader, this->modelData.back());
            break;
        case (ESMType::DATA):
            reader.readSubrecord(this->magicEffectData);
            break;
        default:
            reader.reportError("Unknown subrecord!");
            break;
        }
    }
}
}
