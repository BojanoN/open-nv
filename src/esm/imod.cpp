#include "imod.hpp"

namespace ESM {
ItemMod::ItemMod(ESMReader& reader)
    : Record(reader)
{

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::OBND);
    reader.readSubrecord(objectBounds);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::FULL:
            reader.readStringSubrecord(name);
            break;
        case ESMType::MODL:
            ModelData::load(reader, modelData, 0);
            break;
        case ESMType::ICON:
            reader.readStringSubrecord(largeIconFilename);
            break;
        case ESMType::MICO:
            reader.readStringSubrecord(smallIconFilename);
            break;
        case ESMType::DEST:
            DestructionData::load(reader, destructionData);
            break;
        case ESMType::ZNAM:
            reader.readSubrecord(pickUpSound);
            break;
        case ESMType::YNAM:
            reader.readSubrecord(dropSound);
            break;
        case ESMType::RNAM:
            reader.readSubrecord(loopingSound);
            break;
        case ESMType::SCRI:
            reader.readSubrecord(script);
            break;
        case ESMType::DESC:
            reader.readStringSubrecord(description);
            break;
        case ESMType::DATA:
            reader.readSubrecord(data);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
};