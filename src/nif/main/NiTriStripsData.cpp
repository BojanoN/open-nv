#include "NiTriStripsData.hpp"
NiTriStripsData::NiTriStripsData(NifReader& reader) : NiTriBasedGeomData(reader) {
	reader.read(&numStrips, sizeof(uint16_t), 1);
	stripLengths = new uint16_t[numStrips];
	reader.read(stripLengths, sizeof(uint16_t), numStrips);
	reader.read(&hasPoints, sizeof(nif_bool_t), 1);
if(hasPoints && numStrips != 0) {
	points = new uint16_t*[numStrips];
	for(unsigned int i = 0; i < numStrips; i++) {
		points[i] = new uint16_t[stripLengths[i]];
		reader.read(points[i], sizeof(uint16_t), stripLengths[i]);
	}
}
}
NiObject* NiTriStripsData::create(NifReader& reader) {
	return new NiTriStripsData(reader);
}
NiTriStripsData::~NiTriStripsData() {
	delete[]stripLengths;
if(hasPoints && numStrips != 0) {
	for(unsigned int i = 0; i < numStrips; i++) {
		delete[] points[i];
	}
	delete[] points;
}
}
void NiTriStripsData::resolvePointers(NifData& data) {
	NiTriBasedGeomData::resolvePointers(data);
}
