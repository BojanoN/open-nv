#pragma once

#include <filesystem>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <types/errorpair.hpp>
#include <util/shader.hpp>

namespace fs = std::filesystem;

using namespace Types;

class ShaderManager {
public:
    static ErrorPair<std::shared_ptr<Shader>> getShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        fs::path vertexShaderFullPath   = shaderPath / vertexShader;
        fs::path fragmentShaderFullPath = shaderPath / fragmentShader;

        std::lock_guard<std::mutex> guard(_mutex);

        std::string resourcePath = vertexShaderFullPath.string() + fragmentShaderFullPath.string();

        ErrorPair<std::shared_ptr<Shader>> ret;

        auto res = cache[resourcePath].lock();

        if (!res) {
            try {
                cache[resourcePath] = res = std::shared_ptr<Shader> {
                    new Shader(vertexShaderFullPath.string().c_str(), fragmentShaderFullPath.string().c_str()),
                    [&, resourcePath](Shader* res) {
                        cache.erase(resourcePath);
                        delete res;
                    }
                };
                ret.value = res;
                ret.error = Err::Success;
            } catch (std::runtime_error& e) {
                ret.error = Err::ShaderCreationError;
            }
        }

        return ret;
    }

    static fs::path shaderPath;

private:
    static inline std::mutex                                             _mutex;
    static inline std::unordered_map<std::string, std::weak_ptr<Shader>> cache;
};
