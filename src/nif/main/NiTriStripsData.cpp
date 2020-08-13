#include "NiTriStripsData.hpp"
NiTriStripsData::NiTriStripsData(NifReader& reader) : NiTriBasedGeomData(reader) {
	reader.read(&numStrips, sizeof(uint16_t), 1);
	stripLengths = new uint16_t[numStrips];
	reader.read(stripLengths, sizeof(uint16_t), numStrips);
	reader.read(&hasPoints, sizeof(nif_bool_t), 1);
if(hasPoints && numStrips != 0) {
	points = new uint16_t[numStrips];
	reader.read(points, sizeof(uint16_t), numStrips);
}
}
NiObject* NiTriStripsData::create(NifReader& reader) {
	return new NiTriStripsData(reader);
}
NiTriStripsData::~NiTriStripsData() {
	delete[]stripLengths;
if(hasPoints && numStrips != 0) {
	delete[]points;
}
}
void NiTriStripsData::resolvePointers(NifData& data) {
	NiTriBasedGeomData::resolvePointers(data);
}
