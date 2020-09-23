#include "BSBound.hpp"
BSBound::BSBound(NifReader& reader) : NiExtraData(reader) {
	center.load(reader);
	dimensions.load(reader);
}
NiObject* BSBound::create(NifReader& reader) {
	return new BSBound(reader);
}
BSBound::~BSBound() {
}
void BSBound::resolvePointers(NifData& data) {
	NiExtraData::resolvePointers(data);
	center.resolvePointers(data);
	dimensions.resolvePointers(data);
}
