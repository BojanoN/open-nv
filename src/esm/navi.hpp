#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {
struct __attribute__((packages)) NavigationMapInfo {
    uint32_t             unknown;
    formid               navigationMesh; // NAVM
    formid               location; // CELL or WRLD
    uint16_t             gridX;
    uint16_t             gridY;
    std::vector<uint8_t> unknown;
};
struct NavigationMeshInfoMap : public Record {
    std::string editorId;
    uint32_t    version;
};
}
