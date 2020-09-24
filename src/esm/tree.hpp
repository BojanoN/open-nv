#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "structs.hpp"

#include <cstdint>
#include <string>
#include <sstream>

namespace ESM {

struct TreeData {
	float leafCurvature;
	float minimumLeafAngle;
	float maximumLeafAngle;
	float branchDimmingValue;
	float leafDimmingValue;
	int32_t shadowRadius;
	float rockSpeed;
	float rustleSpeed;
};

struct BillboardDimensions {
	float width;
	float height;
};

struct Tree : public Record {

	ObjectBounds objectBounds;
	ModelData modelData;
	std::string largeIconFilename;
	std::string smallIconFilename;
	std::vector<uint32_t> speedTreeSeeds;
	TreeData treeData;
	BillboardDimensions billboardDimensions;

	Tree(ESMReader& reader);

};

};