#pragma once

#include "esm/types.hpp"

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace GameWorld {

class CellBlock {

public:
    CellBlock()
        : subblocks()
    {
    }
    ~CellBlock() {};
    void insert(uint32_t subblock, uint32_t cellId);

private:
    std::unordered_map<uint32_t, std::unordered_set<uint32_t>> subblocks;
};

class ExteriorWorldSubBlock {

public:
    ExteriorWorldSubBlock() {};
    ~ExteriorWorldSubBlock() {};
    std::unordered_set<uint32_t> cells;
    /// uint16 [2] Y,X;
    uint32_t coords;
};

class ExteriorWorldBlock {

public:
    ExteriorWorldBlock()
        : subblocks()
    {
    }
    ~ExteriorWorldBlock() {};
    std::vector<ExteriorWorldSubBlock> subblocks;
    uint32_t                           coords;
};

}
