#pragma once

#include <filesystem>
#include <util/shader.hpp>

#include <memory>
#include <mutex>
#include <unordered_map>

namespace fs = std::filesystem;

class ShaderManager {
public:
    static std::shared_ptr<Shader> getShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        fs::path vertexShaderFullPath   = shaderPath / vertexShader;
        fs::path fragmentShaderFullPath = shaderPath / fragmentShader;

        std::lock_guard<std::mutex> guard(_mutex);

        std::string resourcePath = vertexShaderFullPath.string() + fragmentShaderFullPath.string();

        auto res = cache[resourcePath].lock();

        if (!res) {
            cache[resourcePath] = res = std::shared_ptr<Shader> {
                new Shader(vertexShaderFullPath.string().c_str(), fragmentShaderFullPath.string().c_str()),
                [&, resourcePath](Shader* res) {
                    cache.erase(resourcePath);
                    delete res;
                }
            };
        }
        return res;
    }
    static fs::path shaderPath;

private:
    static inline std::mutex                                             _mutex;
    static inline std::unordered_map<std::string, std::weak_ptr<Shader>> cache;
};
