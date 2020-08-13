#include "AbstractAdditionalGeometryData.hpp"
AbstractAdditionalGeometryData::AbstractAdditionalGeometryData(NifReader& reader) : NiObject(reader) {
}
NiObject* AbstractAdditionalGeometryData::create(NifReader& reader) {
	return new AbstractAdditionalGeometryData(reader);
}
AbstractAdditionalGeometryData::~AbstractAdditionalGeometryData() {
}
void AbstractAdditionalGeometryData::resolvePointers(NifData& data) {
	NiObject::resolvePointers(data);
}
