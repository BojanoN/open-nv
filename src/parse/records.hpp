#pragma once

#include "data_file.hpp"

class TES4Record : public Record {
public:
    TES4Record(){};

    TES4Record(std::ifstream& stream) : Record(stream)
    {
    }

    virtual Record* clone(std::ifstream& stream)
    {
        return new TES4Record(stream);
    }
};
