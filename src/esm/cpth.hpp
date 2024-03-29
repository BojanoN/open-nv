#pragma once
#include "record.hpp"
#include "subrecords/condition.hpp"
#include "structs.hpp"

namespace ESM {

struct RelatedCameraPaths {
	formid parent; //CPTH
	formid previousSibling; //CPTH
};

enum class CameraZoom : uint8_t {
	Default,
	Disable,
	ShotList
};

struct CameraPath : public Record {

    std::vector<Condition> conditions;
    RelatedCameraPaths relatedCameraPaths;
    CameraZoom 			zoom;
    std::vector<formid> cameraShots; //CAMS

    CameraPath(ESMReader& reader);
};
};