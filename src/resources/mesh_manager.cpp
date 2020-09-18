#include "mesh_manager.hpp"

std::shared_ptr<NifData> MeshManager::getMesh(const std::string& meshPath) {

	auto[cachedMesh, ok] = meshCache.getCachedObject(meshPath);
	if(ok) {
		log_debug("Found %s in cache", meshPath.c_str());
		return cachedMesh;
	}

	log_debug("Reading %s from file", meshPath.c_str());
	std::vector<uint8_t> meshFile = getFileProvider().readFile(PREFIX + meshPath);

	log_debug("Read file of size %d", meshFile.size());

	std::shared_ptr<NifData> mesh = std::make_shared<NifData>(meshFile);
	
	meshCache.insert(meshPath, mesh);
	return mesh;
}