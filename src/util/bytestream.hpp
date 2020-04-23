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

        setg((char*)buffer.data(), (char*)buffer.data(), (char*)buffer.data() + buffer.size());
    }

    std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)
    {
        if (way == std::ios_base::cur && ((off + gptr()) < egptr())) {
            gbump(off);
        } else if (way == std::ios_base::beg && ((off + eback()) < egptr())) {
            setg(eback(), eback() + off, egptr());
        }

        return gptr() - eback();
    };

private:
    std::vector<uint8_t> buffer;
};
