#pragma once
#include <algorithm>
#include <unordered_map>

template <typename K, typename V>
class ResourceCache {

public:

	bool isCached(K key);
	std::pair<V*, bool> getCachedObject(K key);
	void insert(K key, V* value);
	void clear();

private:

	std::unordered_map<K, V*> cacheMap;
};

template<typename K, typename V>
bool ResourceCache<K, V>::isCached(K key) {
	return this->cacheMap.find(key) != this->cacheMap.end();
}

template<typename K, typename V>
std::pair<V*, bool> ResourceCache<K, V>::getCachedObject(K key) {
	auto cachedObject = this->cacheMap.find(key);
	if(cachedObject != this->cacheMap.end()) {
		return std::make_pair(cachedObject->second, true);
	}

	return std::make_pair(nullptr, false);
}

template<typename K, typename V>
void ResourceCache<K, V>::insert(K key, V* value) {
	this->cacheMap.insert(std::make_pair(key, value));
}

template<typename K, typename V>
void ResourceCache<K, V>::clear() {
	this->cacheMap.clear();
}