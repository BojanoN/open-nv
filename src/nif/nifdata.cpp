#include "nifdata.hpp"
#include "main/NiObject.hpp"

NifData::NifData(const std::string& nifPath)
{
    std::vector<uint8_t> nifFileData = getFileProvider().readFile(NifData::meshPathPrefix + nifPath);

    NifReader reader(nifFileData);

    reader.readNifHeader();

    numBlocks = reader.getNumBlocks();
    //blocks = new NiObject*[numBlocks];
    blocks.resize(numBlocks);

    for (unsigned int i = 0; i < numBlocks; i++) {
        NiObject* tmp = reader.readBlock(i);
        blocks[i]     = std::shared_ptr<NiObject>(tmp);
    }

    for (unsigned int i = 0; i < numBlocks; i++) {
        blocks[i]->resolvePointers(*this);
    }
}

NifData::~NifData()
{
    /*if(numBlocks > 50) {
		int a = 5;
	}*/

    /*for(unsigned int i = 0; i < numBlocks; i++) {
		delete blocks[i];
	}
	delete[] blocks;*/
}
