#include "structs.hpp"

static ESMType  filenameType[]    = { ESMType::MODL, ESMType::MOD2, ESMType::MOD3, ESMType::MOD4 };
static uint32_t unusedType[]      = { ESMType.MODB, 0, 0, 0 };
static uint32_t hashesType[]      = { ESMType.MODT, ESMType.MO2T, ESMType.MO3T, ESMType.MO4T };
static uint32_t altTexturesType[] = { ESMType.MODS, ESMType.MO2S, ESMType.MO3S, ESMType.MO4S };
static uint32_t flagType[]        = { ESMType.MODD, 0, ESMType.MOSD, 0 };

void ModelData::load(ESMReader& reader, ModelData& modelData, int index, ESMType nextSubheader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(filenameType[index]);
    reader.readArraySubrecord<char>(modelData.filename.c_str())

        while (reader.subrecordType().intValue != nextSubheader)
    {
        reader.readNextSubrecordHeader();

        if (unusedType[index] && reader.subrecordType().intValue == unusedType[index]) {
            reader.readArraySubrecord<uint8_t>(modelData.unused);

        } else if (reader.subrecordType().intValue == hashesType[index]) {
            //benis

        } else if (reader.subrecordType().intValue == altTexturesType[index]) {
            reader.readRawData<uint32_t>(modelData.alternateTextureCount);
            for (int i = 0; i < modelData.alternateTextureCount; i++) {
                modelData.alternateTextures.push_back(new AlternateTexture);
                AlternateTexture& currentTex = modelData.alternateTextures.back();
                reader.readRawData<uint32_t>(currentTex.nameLength);
                reader.readRawArray<char>(currentTex.name, currentTex.nameLength);
                reader.readRawData<formid>(currentTex.newTexture);
                reader.readRawData<int32_t>(currentTex.index);
            }

        } else if (flagType[index] && reader.subrecordType().intValue == flagType[index]) {
            reader.readSubrecord<uint8_t>(modelData.FaceGenModelFlags);
        }
    }
}
