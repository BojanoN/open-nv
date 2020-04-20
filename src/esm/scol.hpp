#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {
class ESMReader;

struct __attribute__((packed)) PartPlacement {
    float x;
    float y;
    float z;
    float xRot;
    float yRot;
    float zRot;
    float scale;
};

struct StaticCollectionPart {
    formid                     stat;
    std::vector<PartPlacement> placements;
};

struct StaticCollection : public Record {
    std::string                       editorId;
    ObjectBounds                      objectBounds;
    ModelData                         modelData;
    std::vector<StaticCollectionPart> parts;

    StaticCollection(ESMReader& reader);
};
}
