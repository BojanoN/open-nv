#pragma once
#include "zlib/zlib.h"
#include <cstdint>
#include <vector>

namespace Util {

void zlibDecompress(std::vector<uint8_t>& in, std::vector<uint8_t>& out);
};
