#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct AnimatedObject : public Record {
	std::string editorId;
	ModelData 	modelData;
	formid      animation; //IDLE

	AnimatedObject(ESMReader& reader);
};

};