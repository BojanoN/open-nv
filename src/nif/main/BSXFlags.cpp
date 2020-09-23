#include "BSXFlags.hpp"
BSXFlags::BSXFlags(NifReader& reader) : NiIntegerExtraData(reader) {
}
NiObject* BSXFlags::create(NifReader& reader) {
	return new BSXFlags(reader);
}
BSXFlags::~BSXFlags() {
}
void BSXFlags::resolvePointers(NifData& data) {
	NiIntegerExtraData::resolvePointers(data);
}
