#pragma once
#include "acre.hpp"
#include "reader.hpp"
#include "record.hpp"

namespace ESM {

//Identical in structure to ACRE record
struct PlacedNPC : public PlacedCreature {

    PlacedNPC(ESMReader& reader)
        : PlacedCreature(reader)
    {
    }
};

};
