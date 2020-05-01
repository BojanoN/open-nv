#include "cellblock.hpp"

namespace GameWorld {

void CellBlock::insert(uint32_t subblock, uint32_t cellId)
{
    if (subblocks.find(subblock) == subblocks.end()) {
        subblocks[subblock];
    }

    subblocks[subblock].insert(cellId);
}

};
