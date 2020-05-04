#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct RadiationStageData {
	uint32_t triggerThreshold;
	formid   effect; //SPEL
};

struct RadiationStage : public Record {

    std::string   editorId;
    RadiationStageData data;
    
    RadiationStage(ESMReader& reader);
};
};