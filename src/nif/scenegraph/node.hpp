

class SceneNode {

protected:

	glm::mat4 transformation;
	uint32_t numberOfProperties;
	NodeProperty* properties;

public:

	virtual void draw(Renderer& renderer) = 0;

};

class InternalNode : public SceneNode {

private:

	uint32_t numberOfChildren;
	SceneNode** children;

public:

	virtual void draw(Renderer& renderer) {
		for(unsigned int i = 0; i < numberOfChildren; i++) {
			renderer.drawNode(children[i]);
		}
	}
};

class MeshNode : public SceneNode {

private:

	Mesh* mesh;

public:

	virtual void draw(Renderer& renderer) : SceneNode::draw(renderer) {
		mesh->draw(renderer);
	}

};

class NodeProperty {

public:

	virtual void apply(Renderer& renderer) = 0;
};

class AlphaProperty {
public:

	virtual void apply(Renderer& renderer) {
		renderer.setAlphaProperties();
	}
};

class MaterialProperty {

	Color3 specularColor;
	Color3 emissiveColor;
	float glossiness;
	float alpha;
	float emissiveMult;

public:

	virtual void apply(Renderer& renderer) {
		renderer.setSpecularColor();
		// ...
	}
};

class ShaderLightningProperty {

	TextureSet textureSet;

public:

	virtual void apply(Renderer& renderer) {
		renderer.setDiffuseMap(textureSet.getDiffuseMap());
		renderer.setNormalMap(textureSet.getNormalMap());
		// ...
	}
};