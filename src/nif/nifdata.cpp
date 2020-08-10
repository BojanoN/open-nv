#include "nifdata.hpp"



NifData::NifData(const char* filePath) {

	NifReader reader(filePath);

	reader.readNifHeader(this);

	numBlocks = reader.getNumBlocks();
	blocks = new NiObject*[numBlocks];

	for(unsigned int i = 0; i < numBlocks; i++) {
		reader.readBlock(i, blocks[i]);
	}
	for(unsigned int i = 0; i < numBlocks; i++) {
		blocks[i]->resolvePointers(*this);
	}
}



NifData::~NifData() {

	for(unsigned int i = 0; i < numBlocks; i++) {
		delete blocks[i];
	}
	delete[] blocks;
}
