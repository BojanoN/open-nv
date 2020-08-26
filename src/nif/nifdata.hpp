#pragma once


#include "nifreader.hpp"

struct NiObject;

#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

class NifData {

private:
	uint32_t numBlocks;
	NiObject** blocks; // for now

public:

	NifData(const char* filePath);
	~NifData();
	//char* getString(int index);
	//int readData(uint32_t size);
	NiObject* getRoot() { return blocks[0]; } 
	NiObject* getBlock(uint32_t index) { return blocks[index]; }
};