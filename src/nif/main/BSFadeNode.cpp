#include "BSFadeNode.hpp"
BSFadeNode::BSFadeNode(NifReader& reader) : NiNode(reader) {
}
NiObject* BSFadeNode::create(NifReader& reader) {
	return new BSFadeNode(reader);
}
BSFadeNode::~BSFadeNode() {
}
void BSFadeNode::resolvePointers(NifData& data) {
	NiNode::resolvePointers(data);
}
