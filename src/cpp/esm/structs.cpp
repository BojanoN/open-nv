#include "structs.hpp"

namespace ESM {

const static constexpr ESMType filenameType[]    = { ESMType::MODL, ESMType::MOD2, ESMType::MOD3, ESMType::MOD4 };
const static constexpr ESMType unusedType[]      = { ESMType::MODB, ESMType::NO_ENTRY, ESMType::NO_ENTRY, ESMType::NO_ENTRY };
const static constexpr ESMType hashesType[]      = { ESMType::MODT, ESMType::MO2T, ESMType::MO3T, ESMType::MO4T };
const static constexpr ESMType altTexturesType[] = { ESMType::MODS, ESMType::MO2S, ESMType::MO3S, ESMType::MO4S };
const static constexpr ESMType flagType[]        = { ESMType::MODD, ESMType::NO_ENTRY, ESMType::MOSD, ESMType::NO_ENTRY };

void ModelData::load(ESMReader& reader, ModelData& modelData, int index, ESMType nextSubheader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(filenameType[index]);
    reader.readStringSubrecord(modelData.filename);

    while (reader.subrecordType() != nextSubheader) {
        reader.readNextSubrecordHeader();

        if (unusedType[index] && reader.subrecordType() == unusedType[index]) {
            reader.readFixedArraySubrecord(modelData.unused);

        } else if (reader.subrecordType() == hashesType[index]) {
            //benis

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

};
