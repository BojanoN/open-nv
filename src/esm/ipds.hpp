#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {


//all IPCT ids
struct ImpactList {
	formid stone;
	formid dirt;
	formid grass;
	formid glass;
	formid metal;
	formid wood;
	formid organic;
	formid cloth;
	formid water;
	formid hollowMetal;
	formid organicBug;
	formid organicGlow;
};

struct ImpactDataset : public Record {

    std::string  editorId;
    ImpactList   impacts;

    ImpactDataset(ESMReader& reader);
};
};