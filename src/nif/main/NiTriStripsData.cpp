#include "NiTriStripsData.hpp"
NiTriStripsData::NiTriStripsData(NifReader& reader) : NiTriBasedGeomData(reader) {
	reader.read(&numStrips, sizeof(uint16_t), 1);
	stripLengths.resize(numStrips);
	reader.read(&stripLengths[0], sizeof(uint16_t), numStrips);
	reader.read(&hasPoints, sizeof(nif_bool_t), 1);
if(hasPoints && numStrips != 0) {
	points.resize(numStrips);
	for(unsigned int i = 0; i < numStrips; i++) {
		points[i].resize(stripLengths[i]);
		reader.read(&points[i][0], sizeof(uint16_t), stripLengths[i]);
	}
}
}
NiObject* NiTriStripsData::create(NifReader& reader) {
	return new NiTriStripsData(reader);
}
NiTriStripsData::~NiTriStripsData() {
if(hasPoints && numStrips != 0) {
}
}
void NiTriStripsData::resolvePointers(NifData& data) {
	NiTriBasedGeomData::resolvePointers(data);
}
