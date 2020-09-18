#pragma once


#include "nifreader.hpp"

struct NiObject;
//#include "main/NiObject.hpp"

#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <memory>

class NifData {

private:
	uint32_t numBlocks;

	std::vector<std::shared_ptr<NiObject>> blocks;
	//NiObject** blocks; // for now

public:

	NifData(std::vector<uint8_t> nifFileData);
	~NifData();
	NiObject* getRoot() { return blocks[0].get(); } 
	NiObject* getBlock(uint32_t index) { return blocks[index].get(); }
};