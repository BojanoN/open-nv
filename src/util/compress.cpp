#include "compress.hpp"
#include <sstream>
#include <stdexcept>
#include <string>

static std::string zerr(int ret)
{
    std::stringstream s;
    s << "zlib: ";
    switch (ret) {
    case Z_STREAM_ERROR:
        s << "invalid compression level";
        break;
    case Z_DATA_ERROR:
        s << "invalid or incomplete deflate data";
        break;
    case Z_MEM_ERROR:
        s << "out of memory";
        break;
    case Z_VERSION_ERROR:
        s << "zlib version mismatch!";
        break;
    default:
        s << "unknown error!";
        break;
    }
    return s.str();
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
    strm.next_out = Z_NULL;
    ret           = inflateInit(&strm);

    if (ret != Z_OK)
        throw std::runtime_error(zerr(ret));

    strm.next_in  = reinterpret_cast<Bytef*>(&in[0]);
    strm.next_out = reinterpret_cast<Bytef*>(&out[0]);

    strm.avail_out = out.size();
    strm.avail_in  = in.size();

    ret = inflate(&strm, Z_NO_FLUSH);

    if (ret != Z_STREAM_END) {
        (void)inflateEnd(&strm);
        throw std::runtime_error(zerr(ret));
    }

    (void)inflateEnd(&strm);
}
