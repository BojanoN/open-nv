#pragma once


#include "nifreader.hpp"
#include "file/reader.hpp"
#include "resources/resource.hpp"

struct NiObject;

#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <iostream>

class NifData : public File::Reader, public Resource<NifData> {

private:
	uint32_t numBlocks;

	std::vector<std::unique_ptr<NiObject>> blocks;

	static const inline std::string meshPathPrefix = "meshes\\";

	NifData(const std::string& nifPath);

public:

	
	~NifData();
	NiObject* getRoot() { return blocks[0].get(); } 
	NiObject* getBlock(uint32_t index) { return blocks[index].get(); }

	friend class Resource<NifData>;
};