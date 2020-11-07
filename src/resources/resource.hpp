#pragma once
#include <memory>
#include <mutex>
#include <unordered_map>

template <typename T>
class Resource {

    static inline std::mutex                                        _mutex;
    static inline std::unordered_map<std::string, std::weak_ptr<T>> cache;

public:
    static std::shared_ptr<T> get(const std::string& resourcePath);
    static uint64_t           cacheSize() { return cache.size(); }
};

template <typename T>
std::shared_ptr<T> Resource<T>::get(const std::string& resourcePath)
{

    std::lock_guard<std::mutex> guard(Resource<T>::_mutex);

    auto res = Resource<T>::cache[resourcePath].lock();

    if (!res) {
        Resource<T>::cache[resourcePath] = res = std::shared_ptr<T> { new T(resourcePath),
            [&, resourcePath](T* res) {
                Resource<T>::cache.erase(resourcePath);
                delete res;
            } };
    }
    return res;
}
