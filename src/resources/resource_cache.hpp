#pragma once
#include <algorithm>
#include <unordered_map>
#include <memory>

template <typename K, typename V>
class ResourceCache {

public:

	std::pair<std::shared_ptr<V>, bool> getCachedObject(K key);
	void insert(K key, std::shared_ptr<V> value);

private:

	std::unordered_map<K, std::weak_ptr<V>> cacheMap;
};

template<typename K, typename V>
std::pair<std::shared_ptr<V>, bool> ResourceCache<K, V>::getCachedObject(K key) {
	
	auto cachedObject = this->cacheMap.find(key);
	
	if(cachedObject != this->cacheMap.end() && !cachedObject->second.expired()) {
		return std::make_pair(cachedObject->second.lock(), true);
	}

	return std::make_pair(nullptr, false);
}

template<typename K, typename V>
void ResourceCache<K, V>::insert(K key, std::shared_ptr<V> value) {
	this->cacheMap.insert(std::make_pair(key, value));
}

template<typename K, typename V>
void ResourceCache<K, V>::clear() {
	this->cacheMap.clear();
}