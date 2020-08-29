#include "nifdata.hpp"
#include "main/NiObject.hpp"

NifData::NifData(const char* filePath) {

	NifReader reader(filePath);

	reader.readNifHeader();

	numBlocks = reader.getNumBlocks();
	blocks = new NiObject*[numBlocks];
	//blocks.reserve(numBlocks);

	for(unsigned int i = 0; i < numBlocks; i++) {
		blocks[i] = reader.readBlock(i);
		
		if(blocks[i] == NULL) {

			for(unsigned int j = 0; j < i; j++) {
				delete blocks[j];
			}
			delete[] blocks;

			throw std::runtime_error("Cannot read block!");
		}
	}

	for(unsigned int i = 0; i < numBlocks; i++) {
		blocks[i]->resolvePointers(*this);
	}
	for(unsigned int i = 0; i < numBlocks; i++) {
		blocks[i]->resolvePointers(*this);
	}
}



NifData::~NifData() {
	if(numBlocks > 50) {
		int a = 5;
	}

	for(unsigned int i = 0; i < numBlocks; i++) {
		delete blocks[i];
	}
	delete[] blocks;
}
