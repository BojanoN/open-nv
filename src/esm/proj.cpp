#include "proj.hpp"
#include "reader.hpp"

namespace ESM {

Projectile::Projectile(ESMReader& reader)
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
        case ESMType::MODL:
            ModelData::load(reader, this->modelData, 0);
            break;
        case ESMType::DEST:
            DestructionData::load(reader, this->destructionData);
            break;
        case ESMType::DATA:
            // Beams dont have rotation so we just read the data we get
            reader.readRawDataSubrecSize(this->projectileData);
            break;
        case ESMType::NAM1:
            reader.readStringSubrecord(this->muzzleFlashModel);
            break;
        case ESMType::NAM2:
            reader.readArraySubrecord(this->muzzleFlashModelHash);
            break;
        case ESMType::VNAM:
            reader.readSubrecord(this->soundLevel);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
