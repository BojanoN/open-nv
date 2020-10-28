#include "NiNode.hpp"
NiNode::NiNode(NifReader& reader) : NiAVObject(reader) {
	reader.read(&numChildren, sizeof(uint32_t), 1);
children.load(reader, numChildren);
	reader.read(&numEffects, sizeof(uint32_t), 1);
effects.load(reader, numEffects);
}
NiObject* NiNode::create(NifReader& reader) {
	return new NiNode(reader);
}
NiNode::~NiNode() {
}
void NiNode::resolvePointers(NifData& data) {
	NiAVObject::resolvePointers(data);
this->children.resolve(data);
this->effects.resolve(data);
}


Node* NiNode::createGraphNode() {
	Node** nodeChildren = new Node*[numChildren];
	for(unsigned int i = 0; i < numChildren; i++) {
		nodeChildren[i] = children.get(i).createGraphNode();
	}
	NodeProperty* nodeProperties = new NodeProperty[numProperties];
	for(unsigned int i = 0; i < numProperties; i++) {
		nodeProperties[i] = properties.get(i).createNodeProperty();
	}
	return new GroupNode(translation, rotation, scale, numProperties, nodeProperties, numChildren, nodeChildren);
}

