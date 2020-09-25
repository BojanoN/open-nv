#pragma once

#include <string>

namespace BSA {
uint64_t FNVHash(const std::string& path, const std::string& filepath);
uint64_t FNVHashPair(const std::string& fle, const std::string& ext);
}
