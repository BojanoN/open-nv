#pragma once
#include "../file/reader.hpp"
#include "resource_cache.hpp"
#include "nif/nifdata.hpp"
#include <string>
#include <memory>


class MeshManager : public File::Reader {

private:

	ResourceCache<std::string, NifData> meshCache;

	static const inline std::string PREFIX = "meshes\\";

public:

	MeshManager() {}

	std::shared_ptr<NifData> getMesh(const std::string& meshPath);
};