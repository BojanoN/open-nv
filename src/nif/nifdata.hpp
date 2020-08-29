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

	//std::vector<NiObject*> blocks;
	std::vector<std::shared_ptr<NiObject>> blocks;
	//NiObject** blocks; // for now

public:

	NifData(const char* filePath);
	~NifData();
	//char* getString(int index);
	//int readData(uint32_t size);
	//std::shared_ptr<NiObject> getRoot() { return blocks[0]; } 
	//std::shared_ptr<NiObject> getBlock(uint32_t index) { return blocks[index]; }
	NiObject* getRoot() { return blocks[0].get(); } 
	NiObject* getBlock(uint32_t index) { return blocks[index].get(); }
};