#include "lvli.hpp"
#include "reader.hpp"

namespace ESM {

LeveledItem::LeveledItem(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    if (reader.peekNextType() == ESMType::OBND) {
        reader.readNextSubrecordHeader();
        reader.readSubrecord(objectBounds);
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::LVLD);
    reader.readSubrecord(chanceNone);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::LVLF);
    reader.readSubrecord(flags);

    if (reader.peekNextType() == ESMType::LVLG) {
        reader.readNextSubrecordHeader();
        reader.readSubrecord(global);
    }

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
        }
    }
}
}
