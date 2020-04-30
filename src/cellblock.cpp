#include "cellblock.hpp"


namespace GameWorld {

void CellBlock::insert(uint32_t subblock, formid cellId) {
	if(subblocks.find(subblock) == subblocks.end()) {
		subblocks.emplace(subblock);
	}

	subblocks[subblock].insert(cellId);
}

};