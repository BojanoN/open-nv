#include "NiDynamicEffect.hpp"
NiDynamicEffect::NiDynamicEffect(NifReader& reader) : NiAVObject(reader) {
	reader.read(&switchState, sizeof(nif_bool_t), 1);
	reader.read(&numAffectedNodes, sizeof(uint32_t), 1);
affectedNodes.load(reader, numAffectedNodes);
}
NiObject* NiDynamicEffect::create(NifReader& reader) {
	return new NiDynamicEffect(reader);
}
NiDynamicEffect::~NiDynamicEffect() {
}
void NiDynamicEffect::resolvePointers(NifData& data) {
	NiAVObject::resolvePointers(data);
this->affectedNodes.resolve(data);
}
