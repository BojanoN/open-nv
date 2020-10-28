#pragma once

#include <math/mat4.hpp>
#include <math/vec3.hpp>
#include <math/vec4.hpp>

#include <nif/main/NiGeometryData.hpp>

#include <cstdint>

namespace SceneGraph {


class TextureSet {
    Texture2D* diffuse;
    Texture2D* normalSpecular;
    Texture2D* environmentMask;
    Texture2D* glow;
    Texture2D* parallax;
    Texture2D* environment;
};

class NodeProperty {

    virtual void apply();
};

class MaterialProperty : public NodeProperty {
    Color3 specularColor;
    Color3 emissiveColor;
    float glossiness;
    float alpha;
    float emissiveMultiplier;
};

class LightingProperty : public NodeProperty {
    uint32_t textureClampMode;
    TextureSet& textures;
    float parallaxMaxPasses;
    float parallaxScale;
    Color4 emissiveColor;
};

class AlphaProperty : public NodeProperty {
    uint8_t threshold;
}


 
class Node {


public:
    
    /*Node(NiGeometryData& g)
        : geomData(g)
        , transform(1.)
        , scale(1.)
        , colour(1.)
        , dirty(false) {}*/

    Node(Vector3 translation, Matrix33 rotation, float scale, uint32_t numProperties, NodeProperty* properties) : mNumProperties{numProperties}, mProperties{properties} {}

    virtual ~Node()
    {   
        if(mProperties) {
            delete[] mProperties;
        }
    };

    virtual void update(float delta);

protected:
    //NiGeometryData& geomData;
    Matrix44        transform;
    //Vector3         scale;
    Vector4         colour;


    bool dirty;

    uint32_t mNumProperties;
    NodeProperty* mProperties;
};


class GroupNode : public Node {

public:

    GroupNode(Vector3 translation, Matrix33 rotation, float scale, uint32_t numProperties, NodeProperty* properties, uint32_t numChildren, Node** children) : Node(translation, rotation, scale, numProperties, properties), mNumChildren{numChildren}, mChildren{children} {    }

    virtual ~GroupNode() {
        if(mChildren) {
            for (unsigned int i = 0; i < mNumChildren; i++) {
                if(mChildren[i]) {
                    delete mChildren[i];
                }
            }
            delete[] children;
        }
    }

protected:
    uint32_t mNumChildren;
    Node** mChildren;
};

struct GeometryData {

  uint32_t mNumVertices;
  Vector3* mVertices;
  Vector3* mNormals;
  Vector3* mTangents;
  Vector3* mBitangents;
  Color4* mColors;
  TexCoord* mTexCoords;

  GeometryData(uint32_t numVertices,
           Vector3* vertices, Vector3* normals, Vector3* tangents,
           Vector3* bitangents, TexCoord* texCoords, Color4* colors) :  mNumVertices{numVertices} {
    mVertices = new Vector3[numVertices];
    for (unsigned int i = 0; i < numVertices; i++) {
      mVertices[i] = vertices[i];
    }
    if (normals) {
      for (unsigned int i = 0; i < numVertices; i++) {
        mNormals[i] = normals[i];
      }
    }
    if (tangents) {
      for (unsigned int i = 0; i < numVertices; i++) {
        mTangents[i] = tangents[i];
      }
    }
    if (bitangents) {
      for (unsigned int i = 0; i < numVertices; i++) {
        mBitangents[i] = bitangents[i];
      }
    }
    if (texCoords) {
      for (unsigned int i = 0; i < numVertices; i++) {
        mTexCoords[i] = texCoords[i];
      }
    }
    if (colors) {
      for (unsigned int i = 0; i < numVertices; i++) {
        mColors[i] = colors[i];
      }
    }
  }

  ~GeometryData() {
    if (mVertices) {
      delete[] mVertices;
    }
    if (mNormals) {
      delete[] mNormals;
    }
    if (mTangents) {
      delete[] mTangents;
    }
    if (mBitangents) {
      delete[] mBitangents;
    }
    if (mTexCoords) {
      delete[] mTexCoords;
    }
    if (mColors) {
      delete[] mColors;
    }
  }

};

class LeafNode : public Node {
 public:
  LeafNode(Vector3 translation, Matrix33 rotation, float scale, uint32_t numProperties, NodeProperty* properties, GeometryData* geometryData)
      : Node(translation, rotation, scale, numProperties, properties), mGeometryData{geometryData} {

  }

  virtual ~LeafNode() {
    if(geometryData) {
        delete geometryData;
    }
  }

 protected:
    GeometryData* geometryData;
}

class TriStripsNode : public LeafNode {

public:

    TriStripsNode(Vector3 translation, Matrix33 rotation, float scale, uint32_t numProperties, NodeProperty* properties, GeometryData* geometryData, uint16_t numStrips, uint16_t* stripLengths, uint16_t** points) : LeafNode(translation, rotation, scale, geometryData), mNumStrips{numStrips} {
        mStripLengths = new uint16_t[numStrips];
        mPoints = new uint16_t*[numStrips];
        
        for(unsigned int i = 0; i < numStrips; i++) {
            
            mStripLengths[i] = stripLengths[i];
            mPoints[i] = new uint16_t[stripLengths[i]];
            
            for(unsigned int j = 0; j < stripLengths[i]; j++) {
                mPoints[i][j] = points[i][j];    
            }
        }
    }

    virtual ~TriStripsNode() {
        if(mStripLengths) {
            delete[] mStripLengths;
        }
        if(mPoints) {
            for(unsigned int i = 0; i < mNumStrips; i++) {
                if(mPoints[i]) {
                    delete[] mPoints[i];
                }
            }
            delete[] mPoints;
        }
    }

protected:
    uint16_t mNumStrips;
    uint16_t* mStripLengths;
    uint16_t** mPoints;
};

};  // namespace SceneGraph
