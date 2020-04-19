#include "tree.hpp"

namespace ESM {

Tree::Tree(ESMReader& reader)
    : Record(reader)
{

    std::unordered_set<ESMType> modelNextSubheaders = { ESMType::ICON };

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
    reader.checkSubrecordHeader(ESMType::ICON);
    reader.readStringSubrecord(largeIconFilename);

    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::MICO) {
        reader.readStringSubrecord(smallIconFilename);
        reader.readNextSubrecordHeader();
    }

    reader.checkSubrecordHeader(ESMType::SNAM);
    reader.readArraySubrecord(speedTreeSeeds);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::CNAM);
    reader.readSubrecord(treeData);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::BNAM);
    reader.readSubrecord(billboardDimensions);
}

};