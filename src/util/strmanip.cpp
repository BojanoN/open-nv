#include "strmanip.hpp"

void Util::toLowerCase(std::string& str)
{
    int size = str.size();

    for (int i = 0; i < size; i++) {
        str[i] = std::tolower(str[i]);
    }
}
