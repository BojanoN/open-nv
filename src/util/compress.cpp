#include "compress.hpp"
#include <sstream>
#include <stdexcept>
#include <string>

#define CHUNK 4096

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

    uint32_t currIn  = 0;
    uint32_t currOut = 0;

    uint32_t currInAvail = 0;

    Bytef* rawIn  = reinterpret_cast<Bytef*>(&in[0]);
    Bytef* rawOut = reinterpret_cast<Bytef*>(&out[0]);

    strm.zalloc   = Z_NULL;
    strm.zfree    = Z_NULL;
    strm.opaque   = Z_NULL;
    strm.avail_in = 0;
    strm.next_in  = Z_NULL;
    strm.next_out = Z_NULL;
    ret           = inflateInit(&strm);

    if (ret != Z_OK)
        throw std::runtime_error(zerr(ret));

    do {

        if ((currIn + CHUNK) > in.size()) {
            currInAvail = in.size() - currIn;
        } else {
            currInAvail = CHUNK;
        }

        strm.avail_in = currInAvail;
        strm.next_in  = rawIn + currIn;

        do {

            strm.avail_out = CHUNK;
            strm.next_out  = rawOut + currOut;

            ret = inflate(&strm, Z_NO_FLUSH);

            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_STREAM_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                throw std::runtime_error(zerr(ret));
            }
            currOut += CHUNK - strm.avail_out;

        } while (strm.avail_out == 0);
        currIn += currInAvail;

    } while (ret != Z_STREAM_END);

    if (ret != Z_STREAM_END) {
        (void)inflateEnd(&strm);
        throw std::runtime_error(zerr(ret));
    }
    (void)inflateEnd(&strm);
}
