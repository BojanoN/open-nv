#include "pwat.hpp"

namespace ESM {

PlaceableWater::PlaceableWater(ESMReader& reader)
    : Record(reader)
{

    std::unordered_set<ESMType> modelNextSubheaders = { ESMType::DNAM };

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::OBND);
    reader.readSubrecord(objectBounds);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::MODL);
    ModelData::load(reader, modelData, 0);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DNAM);
    reader.readSubrecord(data);

}

};