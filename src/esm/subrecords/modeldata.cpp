#include "modeldata.hpp"

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

    //modelData.loadModel();
}

bool ModelData::isInCollection(uint32_t type, int index)
{
    uint32_t limit;
    switch (index) {
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

    for (uint32_t i = 0; i < limit; i++) {
        if (types[index][i] == type) {
            return true;
        }
    }
    return false;
}
/*
std::shared_ptr<NiObject> ModelData::getModel() {
    
    if(!this->loaded) {
        throw std::logic_error(std::string("Model not loaded: ") + this->filename);
    }

    return model->getRoot();
}*/

void ModelData::loadModel() {
    try {
        model = NifData::get(filename);
    } catch(std::runtime_error& e) {
        log_error("%s", e.what());
        log_error("Cannot read file %s", filename.c_str());
    }
}

ModelData::~ModelData() {
}

}; // namespace