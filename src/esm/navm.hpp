#pragma once
#include "record.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace ESM {

class ESMReader;

struct NavigationMeshData {
    formid   cell;
    uint32_t vertexCount;
    uint32_t triangleCount;
    uint32_t externalConnectionsCount;
    uint32_t NCVACount;
    uint32_t doorsCount;
};

struct NavigationMeshDoor {
    formid   reference; // REFR
    uint16_t triangle;
    uint16_t unused;
};

struct NavigationCell {
    int16_t triangle;
};

struct __attribute__((packed)) ExternalConnections {
    uint32_t unknown;
    formid   navMesh; // NAVM
    uint16_t triangle;
};

struct NavigationMeshGridCoords {
    uint32_t navMeshGridDivisor;
    float    maxXDistance;
    float    maxYDistance;

    float minX;
    float minY;
    float minZ;
    float maxX;
    float maxY;
    float maxZ;
};

struct NavigationMeshGrid {
    NavigationMeshGridCoords    coords;
    std::vector<NavigationCell> cells;

    static void load(ESMReader& reader, std::vector<NavigationMeshGrid>& vector);
};

struct NavigationMesh : public Record {
    std::string           editorId;
    uint32_t              version;
    NavigationMeshData    data;
    std::vector<Vertex>   vertices;
    std::vector<Triangle> triangles;
    // std::vector<int16_t> possiblyTriangleIds;
    std::vector<NavigationMeshDoor>  doors;
    std::vector<NavigationMeshGrid>  meshGrids;
    std::vector<ExternalConnections> externalConnections;

    NavigationMesh(ESMReader& reader);
};

};
