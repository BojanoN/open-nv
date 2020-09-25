#include "nifdata.hpp"
#include "main/NiObject.hpp"

NifData::NifData(const std::string& nifPath)
{
    std::vector<uint8_t> nifFileData = getFileProvider().readFile(NifData::meshPathPrefix + nifPath);

    NifReader reader(nifFileData);

    reader.readNifHeader();

    numBlocks = reader.getNumBlocks();
    blocks.resize(numBlocks);

    for (unsigned int i = 0; i < numBlocks; i++) {
        try {
            blocks[i]     = std::unique_ptr<NiObject>(reader.readBlock(i));
        } catch (std::bad_alloc& badAlloc) {
            log_fatal("Bad alloc while reading %s", nifPath.c_str());
            throw std::runtime_error(badAlloc.what());
        }
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
