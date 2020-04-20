#include "compress.hpp"
#include <stdexcept>
#include <string>

static std::string zerr(int ret)
{

    switch (ret) {
    case Z_STREAM_ERROR:
        return "invalid compression level\n";
    case Z_DATA_ERROR:
        return "invalid or incomplete deflate data\n";
    case Z_MEM_ERROR:
        return "out of memory\n";
    case Z_VERSION_ERROR:
        return "zlib version mismatch!\n";
    default:
        return "unknown error!\n";
    }
}

void Util::zlibDecompress(std::vector<uint8_t>& in, std::vector<uint8_t>& out)
{
    int      ret;
    z_stream strm;

    /* allocate inflate state */
    strm.zalloc   = Z_NULL;
    strm.zfree    = Z_NULL;
    strm.opaque   = Z_NULL;
    strm.avail_in = 0;
    strm.next_in  = Z_NULL;
    ret           = inflateInit(&strm);

    if (ret != Z_OK)
        throw std::runtime_error(zerr(ret));

    ret = inflate(&strm, Z_NO_FLUSH);

    if (ret != Z_STREAM_END) {
        (void)inflateEnd(&strm);
        throw std::runtime_error(zerr(ret));
    }

    (void)inflateEnd(&strm);
}
