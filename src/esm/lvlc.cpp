#include "lvlc.hpp"
#include "reader.hpp"

namespace ESM {

LeveledActor::LeveledActor(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::OBND);
    reader.readSubrecord(objectBounds);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::LVLD);
    reader.readSubrecord(chanceNone);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::LVLF);
    reader.readSubrecord(flags);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();

        switch (reader.subrecordType()) {
        case (ESMType::LVLO):
            leveledList.emplace_back();
            reader.readSubrecord(leveledList.back().baseData);
            break;
        case (ESMType::COED):
            reader.readSubrecord(leveledList.back().extraData);
            break;
        case (ESMType::MODL):
            ModelData::load(reader, modelData, 0);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
