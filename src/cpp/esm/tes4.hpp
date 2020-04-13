#pragma once
#include "record.hpp"
#include <iostream>

namespace ESM {
struct TES4 : public Record {
public:
    TES4(FILE* esm_file)
        : Record(esm_file)
    {
        std::cout << "BENIS\n";
    };
    ~TES4() {};
};
}
