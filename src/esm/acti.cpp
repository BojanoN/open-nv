#include "acti.hpp"

namespace ESM {

Activator::Activator(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::OBND:
            reader.readSubrecord(objectBounds);
            break;
        case ESMType::FULL:
            reader.readStringSubrecord(name);
            break;
        case ESMType::MODL:
            ModelData::load(reader, modelData, 0);
            break;
        case ESMType::SCRI:
            reader.readSubrecord(script);
            break;
        case ESMType::DEST:
            DestructionData::load(reader, destructionData);
            break;
        case ESMType::SNAM:
            reader.readSubrecord(soundLooping);
            break;
        case ESMType::VNAM:
            reader.readSubrecord(soundActivation);
            break;
        case ESMType::INAM:
            reader.readSubrecord(radioTemplate);
            break;
        case ESMType::RNAM:
            reader.readSubrecord(radioStation);
            break;
        case ESMType::WNAM:
            reader.readSubrecord(waterType);
            break;
        case ESMType::XATO:
            reader.readStringSubrecord(activationPrompt);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
};
