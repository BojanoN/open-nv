#include "resource.hpp"

std::shared_ptr<Resource> Resource::get(const std::string& resourcePath) {

	std::lock_guard<std::mutex> guard(Resource::_mutex);

	auto res = Resource::cache[resourcePath].lock();

	std::cout << resourcePath << std::endl;
	if(!res) {
		Resource::cache[resourcePath] = res = std::shared_ptr<Resource>{loadResource(resourcePath), 
			[&, resourcePath](Resource* res) { 
				Resource::cache.erase(resourcePath);
				delete res; 
			}};
	}
	return res;
}