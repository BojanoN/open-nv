#pragma once
#include "reader.hpp"
#include "record.hpp"
#include "acre.hpp"

namespace ESM {

//Identical in structure to ACRE record
struct PlacedNPC : public PlacedCreature {

	PlacedNPC(ESMReader& reader) : PlacedCreature(reader) {}
};

};