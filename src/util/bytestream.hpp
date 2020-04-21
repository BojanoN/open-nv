#pragma once
#include <istream>

class bytebuf : public std::basic_streambuf<char> {
public:
    bytebuf(std::vector<uint8_t> vec)
    {
        setg((char*)&vec[0], (char*)&vec[0], (char*)&vec[0] + vec.size());
    }
};

class bytestream : public std::istream {
public:
    bytestream(std::vector<uint8_t> vec)
        : std::istream(&_buf)
        , _buf(vec)
    {
        rdbuf(&_buf);
    }

private:
    bytebuf _buf;
};
