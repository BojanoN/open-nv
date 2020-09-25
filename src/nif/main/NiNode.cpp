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
