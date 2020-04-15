#pragma once

#include "reader.hpp"
#include "record.hpp"
#include <iostream>

namespace ESM {

struct TES4 : public Record {
public:
	
    ~TES4() {};
    void load(ESMReader& reader);
};
}
