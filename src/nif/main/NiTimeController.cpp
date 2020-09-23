#include "NiTimeController.hpp"
NiTimeController::NiTimeController(NifReader& reader) : NiObject(reader) {
nextController.load(reader);
	uint16_t flagsData;
	reader.read(&flagsData, sizeof(uint16_t), 1);
	flags = flagsData;
	reader.read(&frequency, sizeof(float), 1);
	reader.read(&phase, sizeof(float), 1);
	reader.read(&startTime, sizeof(float), 1);
	reader.read(&stopTime, sizeof(float), 1);
target.load(reader);
}
NiObject* NiTimeController::create(NifReader& reader) {
	return new NiTimeController(reader);
}
NiTimeController::~NiTimeController() {
}
void NiTimeController::resolvePointers(NifData& data) {
	NiObject::resolvePointers(data);
this->nextController.resolve(data);
this->target.resolve(data);
}
