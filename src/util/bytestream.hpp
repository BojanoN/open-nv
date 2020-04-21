#pragma once
#include "util/compress.hpp"
#include <istream>
#include <vector>

class decompBuf : public std::basic_streambuf<char> {
public:
    decompBuf(std::istream& in, uint32_t compSize, uint32_t decompSize)
        : buffer(decompSize)
    {
        std::vector<uint8_t> tmp(compSize);
        in.read(reinterpret_cast<char*>(&tmp[0]), compSize);
        ::Util::zlibDecompress(tmp, buffer);

        setg((char*)&buffer[0], (char*)&buffer[0], (char*)&buffer[0] + decompSize);
    }

private:
    std::vector<uint8_t> buffer;
};
