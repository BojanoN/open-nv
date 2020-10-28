#include "NiTriStrips.hpp"
NiTriStrips::NiTriStrips(NifReader& reader) : NiTriBasedGeom(reader) {
}
NiObject* NiTriStrips::create(NifReader& reader) {
	return new NiTriStrips(reader);
}
NiTriStrips::~NiTriStrips() {
}
void NiTriStrips::resolvePointers(NifData& data) {
	NiTriBasedGeom::resolvePointers(data);
}

Node* NiTriStrips::createGraphNode() {
	NodeProperty* nodeProperties = new NodeProperty[numProperties];
	for(unsigned int i = 0; i < numProperties; i++) {
		nodeProperties[i] = properties.get(i).createNodeProperty();
	}
	return this->data.get().createLeafNode(translation, rotation, scale, numProperties, properties);
}
