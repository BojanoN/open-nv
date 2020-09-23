#include "NiObjectNET.hpp"
NiObjectNET::NiObjectNET(NifReader& reader) : NiObject(reader) {
	name = reader.readIndexedString();
	reader.read(&numExtraDataList, sizeof(uint32_t), 1);
extraDataList.load(reader, numExtraDataList);
controller.load(reader);
}
NiObject* NiObjectNET::create(NifReader& reader) {
	return new NiObjectNET(reader);
}
NiObjectNET::~NiObjectNET() {
}
void NiObjectNET::resolvePointers(NifData& data) {
	NiObject::resolvePointers(data);
this->extraDataList.resolve(data);
this->controller.resolve(data);
}
