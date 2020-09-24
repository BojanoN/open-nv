#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "subrecords/destructiondata.hpp"
#include "structs.hpp"

#include <cstdint>
#include <string>
#include <unordered_set>

namespace ESM {

struct TalkingActivator : public Record {

	ObjectBounds objectBounds;
	std::string name;
	ModelData modelData;
	formid script;
	DestructionData destructionData;
	formid soundLooping;
	formid voiceType;
	formid radioTemplate;

	TalkingActivator(ESMReader& reader);
};

};