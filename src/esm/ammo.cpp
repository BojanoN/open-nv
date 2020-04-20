#include "ammo.hpp"
#include "reader.hpp"

namespace ESM {

Ammo::Ammo(ESMReader& reader)
    : Record(reader)
{

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::OBND:
            reader.readSubrecord(this->objectBounds);
            break;
        case ESMType::FULL:
            reader.readStringSubrecord(this->name);
            break;
        case ESMType::ICON:
            reader.readStringSubrecord(this->largeIconFilename);
            break;
        case ESMType::MICO:
            reader.readStringSubrecord(this->smallIconFilename);
            break;
        case ESMType::MODL:
            ModelData::load(reader, this->modelData, 0);
            break;
        case ESMType::DEST:
            DestructionData::load(reader, this->destructionData);
            break;
        case ESMType::SCRI:
            reader.readSubrecord(this->script);
            break;
        case ESMType::YNAM:
            reader.readSubrecord(this->pickUpSound);
            break;
        case ESMType::ZNAM:
            reader.readSubrecord(this->dropSound);
            break;
        case ESMType::ONAM:
            reader.readStringSubrecord(this->shortName);
            break;
        case ESMType::QNAM:
            reader.readStringSubrecord(this->abbreviation);
            break;
        case ESMType::RCIL:
            reader.readSubrecord(this->ammoEffect);
            break;
        case ESMType::DATA:
            reader.readSubrecord(this->data);
            break;
        case ESMType::DAT2: {
            uint16_t dataSize = reader.getCurrentSubrecord().dataSize;
            if (dataSize == sizeof(AmmoProjectileData)) {
                this->projectileData.emplace<0>();
                reader.readSubrecord(std::get<AmmoProjectileData>(this->projectileData));
            } else if (dataSize == sizeof(AmmoNonProjectileData)) {
                this->projectileData.emplace<1>();
                reader.readSubrecord(std::get<AmmoNonProjectileData>(this->projectileData));
            }
            break;
        }
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
