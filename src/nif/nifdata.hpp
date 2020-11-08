#pragma once

#include "file/reader.hpp"
#include "nifreader.hpp"
#include "resources/resource.hpp"

struct NiObject;

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>

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
