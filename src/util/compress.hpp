#pragma once
#include <cstdint>
#include <vector>
#include <zlib.h>

namespace Util {

void zlibDecompress(std::vector<uint8_t>& in, std::vector<uint8_t>& out);
};
