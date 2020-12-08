#pragma once
#include "renderer.hpp"
#include <cstdint>


// Make the mesh depend on OpenGL (keep VAO, VBO etc.)?

class Mesh {

protected:

	uint16_t numberOfVertices;
	//number of vertices
	float* vertices;
	bool hasNormals;
	//number of vertices
	float* normals;
	float* tangents;
	float* bitangents;
	bool hasColors;
	float* colors;
	float* textureCoordinates;

public:

	virtual void draw(Renderer& renderer) = 0;

};

class TriStripsMesh : public Mesh {

protected:
	uint16_t numberOfStrips;
	//number of strips
	uint16_t* stripLengths;
	//sum(strip lengths)
	uint16_t* stripPoints;

public:

	virtual void draw(Renderer& renderer) {
		renderer.bind(this);
		renderer.draw();
		rendere.unbind();
	}

}

/**
	int32_t groupID;
	uint16_t numVertices;
	uint16_t bsMaxVertices;
	uint8_t keepFlags;
	uint8_t compressFlags;
	nif_bool_t hasVertices;
	std::vector<Vector3> vertices;
	BSGeometryDataFlags bsDataFlags;
	nif_bool_t hasNormals;
	std::vector<Vector3> normals;
	std::vector<Vector3> tangents;
	std::vector<Vector3> bitangents;
	NiBound boundingSphere;
	nif_bool_t hasVertexColors;
	std::vector<Color4> vertexColors;
	std::vector<TexCoord> uvSets;
	NiEnums::ConsistencyType consistencyFlags;
	NifPointer<AbstractAdditionalGeometryData> additionalData;


	uint16_t numStrips;
	std::vector<uint16_t> stripLengths;
	nif_bool_t hasPoints;
	std::vector<std::vector<uint16_t>> points;
	*/
