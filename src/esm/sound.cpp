#include "sound.hpp"
#include "reader.hpp"

namespace ESM {
Sound::Sound(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case (ESMType::OBND):
            reader.readSubrecord(this->objectBounds);
            break;
        case (ESMType::FNAM):
            reader.readStringSubrecord(this->soundFilename);
            break;
        case (ESMType::RNAM):
            reader.readSubrecord(this->randomChangePercentage);
            break;
        case (ESMType::SNDX):
            reader.readSubrecord(this->soundData.soundEx);
            break;
        case (ESMType::SNDD):
            reader.readSubrecord(this->soundData);
            break;
        case (ESMType::ANAM):
            reader.readFixedArraySubrecord(this->soundData.attenuationPoints);
            break;
        case (ESMType::GNAM):
            reader.readSubrecord(this->soundData.reverbAttenuationControl);
            break;
        case (ESMType::HNAM):
            reader.readSubrecord(this->soundData.priority);
            break;
        }
    }
}
}
