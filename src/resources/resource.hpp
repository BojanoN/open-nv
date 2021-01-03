#pragma once
#include <memory>
#include <mutex>
#include <unordered_map>

#include <types/errorpair.hpp>

template <typename T>
class Resource {

    static inline std::mutex                                        _mutex;
    static inline std::unordered_map<std::string, std::weak_ptr<T>> cache;

public:
    static Types::ErrorPair<std::shared_ptr<T>> get(const std::string& resourcePath);
    static uint64_t                             cacheSize() { return cache.size(); }
};

template <typename T>
Types::ErrorPair<std::shared_ptr<T>> Resource<T>::get(const std::string& resourcePath)
{

    std::lock_guard<std::mutex> guard(Resource<T>::_mutex);

    auto res = Resource<T>::cache[resourcePath].lock();

    Types::ErrorPair<std::shared_ptr<T>> ret;

    if (!res) {
        try {
            Resource<T>::cache[resourcePath] = res = std::shared_ptr<T> {
                new T(resourcePath),
                [&, resourcePath](T* res) {
                    Resource<T>::cache.erase(resourcePath);
                    delete res;
                }
            };

            ret.value = res;
            ret.error = Err::Success;
        } catch (std::runtime_error& e) {
            ret.error = Err::ResourceCreationError;
        }
    }

    return ret;
}

// NB: maximum allowed number of distinct resource type is 4096
enum class ResourceType : uint16_t {
    VertexBuffer,
    VertexArray,
    ElementBuffer
};
