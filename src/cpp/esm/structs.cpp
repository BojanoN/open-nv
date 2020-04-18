#include "structs.hpp"

namespace ESM {

void ModelData::load(ESMReader& reader, ModelData& modelData, int index, std::unordered_set<ESMType>& nextSubheaders)
{
    reader.checkSubrecordHeader(filenameType[index]);
    reader.readStringSubrecord(modelData.filename);

    while (reader.hasMoreSubrecords() && nextSubheaders.find(static_cast<ESMType>(reader.peekNextType())) == nextSubheaders.end()) {
        reader.readNextSubrecordHeader();

        if (unusedType[index] && reader.subrecordType() == unusedType[index]) {
            reader.readFixedArraySubrecord(modelData.unused);

        } else if (reader.subrecordType() == hashesType[index]) {
            reader.readArraySubrecord(modelData.textureHashes);

        } else if (reader.subrecordType() == altTexturesType[index]) {
            reader.readRawData(modelData.alternateTextureCount);
            for (int i = 0; i < modelData.alternateTextureCount; i++) {
                modelData.alternateTextures.emplace_back();
                AlternateTexture& currentTex = modelData.alternateTextures.back();
                reader.readRawData(currentTex.nameLength);
                currentTex.name.resize(currentTex.nameLength);
                reader.readFixedSizeString(currentTex.name, currentTex.nameLength);
                reader.readRawData(currentTex.newTexture);
                reader.readRawData(currentTex.index);
            }

        } else if (flagType[index] && reader.subrecordType() == flagType[index]) {
            reader.readSubrecord(modelData.FaceGenModelFlags);
        }
    }
    //reader.rewind(sizeof(SubrecordHeader));
}


void DestructionData::load(ESMReader& reader, DestructionData& destData,
    std::unordered_set<ESMType>& nextSubheaders)
{
    reader.checkSubrecordHeader(ESMType::DEST);
    reader.readSubrecord(destData.header);

    while (reader.hasMoreSubrecords() && nextSubheaders.find(static_cast<ESMType>(reader.peekNextType())) == nextSubheaders.end()) {
        reader.readNextSubrecordHeader();
        reader.checkSubrecordHeader(ESMType::DSTD);

        destData.stages.emplace_back();
        reader.readSubrecord(destData.stages.back().stageData);
        bool stageEnd = false;

        while (reader.hasMoreSubrecords() && nextSubheaders.find(static_cast<ESMType>(reader.peekNextType())) == nextSubheaders.end() && !stageEnd) {

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

void Condition::load(ESMReader& reader, Condition& condition)
{
    if (reader.getCurrentSubrecord().dataSize == 28) {
        reader.readSubrecord(*reinterpret_cast<Condition::ConditionNullReference*>(&condition));
    } else {
        reader.readSubrecord(condition);
    }
}

};
