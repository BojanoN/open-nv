#pragma once

#include <math/mat4.hpp>
#include <math/vec3.hpp>
#include <math/vec4.hpp>

#include <nif/main/NiGeometryData.hpp>

#include <vector>

namespace SceneGraph {
class SceneNode {
public:
    SceneNode(NiGeometryData& g)
        : geomData(g
        , transform(1.)
        , scale(1.)
        , colour(1.)
        , dirty(false) {};
    ~SceneNode()
    {
        for (unsigned int i = 0; i < children.size(); i++) {
            delete children[i];
        }
    };

    virtual void update(float delta);

protected:
    NiGeometryData& geomData;
    Matrix44        transform;
    Vector3         scale;
    Vector4         colour;

    bool dirty;

    std::vector<SceneNode*> children;
};
};
