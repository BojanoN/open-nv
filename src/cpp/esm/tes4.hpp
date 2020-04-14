#pragma once

#include "reader.hpp"
#include "record.hpp"
#include <iostream>

namespace ESM::Records {

struct TES4 : public Record {
public:
    TES4(ESMReader& reader)
        : Record(reader)
    {
        std::cout << "BENIS\n";
    };
    ~TES4() {};
    void load(ESMReader& reader);
};
}
