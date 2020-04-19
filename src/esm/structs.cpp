#include "structs.hpp"

namespace ESM {

void ModelData::load(ESMReader& reader, ModelData& modelData, int index)
{
    reader.checkSubrecordHeader(filenameType[index]);
    reader.readStringSubrecord(modelData.filename);

    while (reader.hasMoreSubrecords() && isInCollection(reader.peekNextType(), index)) {
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
}

bool ModelData::isInCollection(uint32_t type, int index)
{
        uint32_t limit;
        switch(index) {
            case 0:
                limit = 5;
                break;
            case 1:
            case 3:
                limit = 3;
                break;
            case 2:
                limit = 4;
                break;
            default:
                throw std::runtime_error("benis");
        }

        for(uint32_t i = 0; i < limit; i++) {
            if(types[index][i] == type) {
                return true;
            }
        }
        return false;
    }

bool DestructionData::isInCollection(uint32_t type) {
    for(int i = 0; i < 4; i++) {
        if(types[i] == type) {
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

void Condition::load(ESMReader& reader, Condition& condition)
{
    if (reader.getCurrentSubrecord().dataSize == 28) {
        reader.readSubrecord(*reinterpret_cast<Condition::ConditionNullReference*>(&condition));
    } else {
        reader.readSubrecord(condition);
    }
}

void ArmorAttributes::load(ESMReader& reader, ArmorAttributes& attrs)
{
    if (reader.getCurrentSubrecord().dataSize == 4) {
        reader.readSubrecord(*reinterpret_cast<ArmorAttributes::ArmorAttributesSmall*>(&attrs));
    } else {
        reader.readSubrecord(attrs);
    }
}


};
