#include "structs.hpp"

namespace ESM {

const static constexpr ESMType filenameType[]    = { ESMType::MODL, ESMType::MOD2, ESMType::MOD3, ESMType::MOD4 };
const static constexpr ESMType unusedType[]      = { ESMType::MODB, ESMType::NO_ENTRY, ESMType::NO_ENTRY, ESMType::NO_ENTRY };
const static constexpr ESMType hashesType[]      = { ESMType::MODT, ESMType::MO2T, ESMType::MO3T, ESMType::MO4T };
const static constexpr ESMType altTexturesType[] = { ESMType::MODS, ESMType::MO2S, ESMType::MO3S, ESMType::MO4S };
const static constexpr ESMType flagType[]        = { ESMType::MODD, ESMType::NO_ENTRY, ESMType::MOSD, ESMType::NO_ENTRY };

void ModelData::loadCollection(ESMReader& reader, ModelData& modelData)
{

    uint32_t fnameInd  = 0;
    uint32_t hashInd   = 0;
    uint32_t altTexInd = 0;
    uint32_t flagInd   = 0;

    reader.checkSubrecordHeader(filenameType[fnameInd]);

    while (true) {
        uint32_t current = reader.getCurrentSubrecord().type;

        if (current == filenameType[fnameInd]) {
            modelData.filenames.emplace_back();
            reader.readStringSubrecord(modelData.filenames.back());
            fnameInd++;
        } else if (current == hashesType[hashInd]) {
            uint32_t size = reader.getCurrentSubrecord().dataSize;
            modelData.textureHashes.emplace_back();
            modelData.textureHashes.back().resize(size);
            reader.readRawArray(&modelData.textureHashes.back()[0], size);

            hashInd++;

        } else if (current == altTexturesType[altTexInd]) {
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
            altTexInd++;
        } else if (current == flagType[flagInd]) {
            reader.readSubrecord(modelData.FaceGenModelFlags);
            flagInd++;
        } else {
            break;
        }
        reader.readNextSubrecordHeader();
    }
    reader.rewind(sizeof(SubrecordHeader));
}

void ModelData::load(ESMReader& reader, ModelData& modelData, int index, ESMType nextSubheader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::MODL);
    modelData.filenames.emplace_back();
    reader.readStringSubrecord(modelData.filenames.back());

    while (reader.subrecordType() != nextSubheader) {

        if (unusedType[index] && reader.subrecordType() == unusedType[index]) {
            reader.readFixedArraySubrecord(modelData.unused);

        } else if (reader.subrecordType() == hashesType[index]) {
            uint32_t size = reader.getCurrentSubrecord().dataSize;
            modelData.textureHashes.emplace_back();
            modelData.textureHashes.back().resize(size);

            reader.readRawArray(&modelData.textureHashes.back()[0], size);

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
        reader.readNextSubrecordHeader();
    }
    reader.rewind(sizeof(SubrecordHeader));
}

};
