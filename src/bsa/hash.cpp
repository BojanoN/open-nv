#include <algorithm>
#include <cstdint>
#include <string.h>
#include <string>

namespace BSA {

uint32_t FNVHashStr(std::string s)
{
    uint32_t hash = 0;

    for (size_t i = 0; i < s.length(); i++) {
        hash *= 0x1003F;
        hash += (uint8_t)s[i];
    }

    return hash;
}

uint64_t FNVHashPair(std::string fle, std::string ext)
{
    uint64_t hash = 0;

    if (fle.length() > 0) {
        hash = (uint64_t)((((uint8_t)fle[fle.length() - 1]) * 0x1) + ((fle.length() > 2 ? (uint8_t)fle[fle.length() - 2] : (uint8_t)0) * 0x100) + (fle.length() * 0x10000) + (((uint8_t)fle[0]) * 0x1000000));

        if (fle.length() > 3) {
            hash += (uint64_t)(FNVHashStr(fle.substr(1, fle.length() - 3)) * 0x100000000);
        }
    }

    if (ext.length() > 0) {
        hash += (uint64_t)(FNVHashStr(ext) * 0x100000000LL);

        uint8_t i = 0;
        if (ext == ".nif")
            i = 1;
        if (ext == ".kf")
            i = 2;
        if (ext == ".dds")
            i = 3;
        if (ext == ".wav")
            i = 4;

        if (i != 0) {
            uint8_t a = (uint8_t)(((i & 0xfc) << 5) + (uint8_t)((hash & 0xff000000) >> 24));
            uint8_t b = (uint8_t)(((i & 0xfe) << 6) + (uint8_t)(hash & 0x000000ff));
            uint8_t c = (uint8_t)((i << 7) + (uint8_t)((hash & 0x0000ff00) >> 8));

            hash -= hash & 0xFF00FFFF;
            hash += (unsigned int)((a << 24) + b + (c << 8));
        }
    }

    return hash;
}

uint64_t FNVHash(std::string path, std::string file)
{
    std::transform(file.begin(), file.end(), file.begin(), ::tolower);
    std::replace(file.begin(), file.end(), '/', '\\');

    std::string fle;
    std::string ext;

    const char* _fle = file.data();
    const char* _ext = strrchr(_fle, '.');
    if (_ext) {
        ext = file.substr((0 + _ext) - _fle);
        fle = file.substr(0, (_ext)-_fle);
    } else {
        ext = "";
        fle = file;
    }
    if (path.length() && fle.length())
        return FNVHashPair(path + "\\" + fle, ext);
    else
        return FNVHashPair(path + fle, ext);
    return 0;
}
}
