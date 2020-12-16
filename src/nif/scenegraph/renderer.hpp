#pragma once
class Mesh;

#include "mesh.hpp"

class Renderer {

	//probably not
	Mesh* currentMesh;

	// shaderi
	// teksture
	// display konfiguracija


	void bind(Mesh* mesh) {
		currentMesh = mesh;
	}

	void draw() {
		// renders
	}

	void unbind() {
		currentMesh = NULL;
	}

	void applyTransformation(glm::mat4 transformation);

	void drawNode(SceneNode* node) {
		node->getTransformation()
		//transformation setup
		for(unsigned int i = 0; i < node->numberOfProperties; i++) {
			node->properties[i]->apply(*this);
		}

		node->draw(*this);

		//pop transformation and properties?
	}

	void setAlphaProperties();
	void setDiffuseMap();
	void setNormalMap();
	void setSpecularColor();
};

