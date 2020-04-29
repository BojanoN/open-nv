#include "utils.hpp"

namespace ESM::Util {

std::string typeValueToName(uint32_t value)
{
    std::stringstream s;
    s << static_cast<char>(value) << static_cast<char>(value >> 8) << static_cast<char>(value >> 16) << static_cast<char>(value >> 24);
    return s.str();
}

};
