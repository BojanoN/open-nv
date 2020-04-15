/*#include "structs.hpp"

namespace ESM {

void ModelData::load(ESMReader& reader, ModelData& modelData, int index, ESMType nextSubheader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(filenameType[index]);
    reader.readStringSubrecord(modelData.filename);

        while (reader.subrecordType() != nextSubheader)
    {
        reader.readNextSubrecordHeader();

        if (unusedType[index] && reader.subrecordType() == unusedType[index]) {
            reader.readFixedArraySubrecord(modelData.unused);

        } else if (reader.subrecordType() == hashesType[index]) {
            //benis

        } else if (reader.subrecordType() == altTexturesType[index]) {
            reader.readRawData(modelData.alternateTextureCount);
            for (int i = 0; i < modelData.alternateTextureCount; i++) {
                AlternateTexture tex;
                modelData.alternateTextures.push_back(tex);
                AlternateTexture& currentTex = modelData.alternateTextures.back();
                reader.readRawData(currentTex.nameLength);
                reader.readRawArray(currentTex.name, currentTex.nameLength);
                reader.readRawData(currentTex.newTexture);
                reader.readRawData(currentTex.index);
            }

        } else if (flagType[index] && reader.subrecordType() == flagType[index]) {
            reader.readSubrecord(modelData.FaceGenModelFlags);
        }
    }
}

};*/
