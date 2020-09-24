#include "destructiondata.hpp"

namespace ESM {

bool DestructionData::isInCollection(uint32_t type)
{
    for (int i = 0; i < 4; i++) {
        if (types[i] == type) {
            return true;
        }
    }
    return false;
}

void DestructionData::load(ESMReader& reader, DestructionData& destData)
{
    reader.checkSubrecordHeader(ESMType::DEST);
    reader.readSubrecord(destData.header);

    while (reader.hasMoreSubrecords() && isInCollection(reader.peekNextType())) {
        reader.readNextSubrecordHeader();
        reader.checkSubrecordHeader(ESMType::DSTD);

        destData.stages.emplace_back();
        reader.readSubrecord(destData.stages.back().stageData);
        bool stageEnd = false;

        while (reader.hasMoreSubrecords() && isInCollection(reader.peekNextType()) && !stageEnd) {

            reader.readNextSubrecordHeader();
            if (reader.subrecordType() == ESMType::DMDL) {
                reader.readStringSubrecord(destData.stages.back().modelFilename);

            } else if (reader.subrecordType() == ESMType::DMDT) {
                reader.readArraySubrecord(destData.stages.back().textureHashes);
            } else if (reader.subrecordType() == ESMType::DSTF) {
                stageEnd = true;
            }
        }
    }
}

}; // namespace ESM