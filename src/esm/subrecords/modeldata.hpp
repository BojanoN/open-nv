#pragma once

#include "../reader.hpp"
#include "../types.hpp"
#include "nif/nifdata.hpp"
#include <string>
#include <vector>


class NiObject;


namespace ESM {

struct AlternateTexture {
    uint32_t    nameLength;
    std::string name;
    formid      newTexture;
    int32_t     index;
};

struct ModelData {
    std::string                   filename;
    uint8_t                       unused[4];
    uint32_t                      alternateTextureCount;
    std::vector<uint8_t>          textureHashes;
    std::vector<AlternateTexture> alternateTextures;
    uint8_t                       FaceGenModelFlags;


    const NiObject* getModel();
    void            loadModel();
    ~ModelData();

    static void load(ESMReader& reader, ModelData& modelData, int index);

private:
    const static constexpr ESMType filenameType[]    = { ESMType::MODL, ESMType::MOD2, ESMType::MOD3, ESMType::MOD4 };
    const static constexpr ESMType unusedType[]      = { ESMType::MODB, ESMType::NO_ENTRY, ESMType::NO_ENTRY, ESMType::NO_ENTRY };
    const static constexpr ESMType hashesType[]      = { ESMType::MODT, ESMType::MO2T, ESMType::MO3T, ESMType::MO4T };
    const static constexpr ESMType altTexturesType[] = { ESMType::MODS, ESMType::MO2S, ESMType::MO3S, ESMType::MO4S };
    const static constexpr ESMType flagType[]        = { ESMType::MODD, ESMType::NO_ENTRY, ESMType::MOSD, ESMType::NO_ENTRY };

    const static constexpr ESMType index_0[] = { ESMType::MODL, ESMType::MODB, ESMType::MODT, ESMType::MODS, ESMType::MODD };
    const static constexpr ESMType index_1[] = { ESMType::MOD2, ESMType::MO2T, ESMType::MO2S };
    const static constexpr ESMType index_2[] = { ESMType::MOD3, ESMType::MO3T, ESMType::MO3S, ESMType::MOSD };
    const static constexpr ESMType index_3[] = { ESMType::MOD4, ESMType::MO4T, ESMType::MO4S };

    const static constexpr ESMType* types[4] = { index_0, index_1, index_2, index_3 };
    static bool                     isInCollection(uint32_t type, int index);

    NifData*    model = NULL;
    bool              loaded = false;
};

}; // namespace