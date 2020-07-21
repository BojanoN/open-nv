#pragma once
#include "record.hpp"
#include "reader.hpp"
#include "structs.hpp"

namespace ESM {

struct AnimatedObject : public Record {
	ModelData 	modelData;
	formid      animation; //IDLE

	AnimatedObject(ESMReader& reader);
};

};