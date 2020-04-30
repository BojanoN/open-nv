#pragma once

#include "esm/types.hpp"

#include <unordered_map>
#include <unordered_set>

namespace GameWorld {

class CellBlock {

public:
	CellBlock() {}
	void insert(uint32_t subblock, formid cellId);

private:
	std::unordered_map<uint32_t, std::unordered_set<formid>> subblocks;

};

}