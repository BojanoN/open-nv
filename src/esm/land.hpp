#pragma once
#include "record.hpp"
#include "structs.hpp"

#include <vector>

#define LANDSCAPE_VERTEX_COUNT 1089

namespace ESM {

class ESMReader;

enum class LayerQuadrant : uint8_t {
    BottomLeft,
    BottomRight,
    TopLeft,
    TopRight
};

struct LayerHeader {
    formid        texture;
    LayerQuadrant quadrant;
    uint8_t       unused;
    int16_t       layer;
};

struct AlphaLayerCellData {
    uint16_t position;
    uint16_t unused;
    float    opacity;
};

struct BaseLayer {
    LayerHeader header;
};

struct AlphaLayer {
    LayerHeader        header;
    AlphaLayerCellData data;
};

struct VertexHeightMap {
    float   offset;
    uint8_t rows[LANDSCAPE_VERTEX_COUNT];
    uint8_t unused[3];
};

struct Landscape : public Record {
    Vertex                  vertexNormals[LANDSCAPE_VERTEX_COUNT];
    VertexHeightMap         vertexHeightMap;
    std::vector<BaseLayer>  baseLayers;
    std::vector<AlphaLayer> alphaLayers;
    rgb                     vertexColors[LANDSCAPE_VERTEX_COUNT];
    std::vector<formid>     textures; // LTEX

    Landscape(ESMReader& reader);
};

}
