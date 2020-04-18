#include "record.hpp"
#include "types.hpp"

namespace ESM {
class ESMReader;

struct __attribute__((packed)) HavokData {
    uint8_t materialType;
    uint8_t friction;
    uint8_t restitution;
};

struct LandscapeTexture : public Record {
    std::string editorId;
    std::string largeIconFilename;
    std::string smallIconFilename;
    // TXST
    formid    texture;
    HavokData havok;
    uint8_t   textureSpecularExponent;
    // GRAS
    std::vector<formid> grass;

    LandscapeTexture(ESMReader& reader);
};
}
