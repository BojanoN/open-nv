#pragma once

#include <string>

namespace BSA {
uint64_t FNVHash(std::string path, std::string file);
uint64_t FNVHashPair(std::string fle, std::string ext);
}
