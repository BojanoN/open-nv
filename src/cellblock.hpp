#pragma once

#include "esm/types.hpp"

#include <unordered_map>
#include <unordered_set>

namespace GameWorld {

class CellBlock {

public:
	CellBlock() : subblocks() {}
	void insert(uint32_t subblock, uint32_t cellId);

private:
	std::unordered_map<uint32_t, std::unordered_set<uint32_t>> subblocks;

};

}