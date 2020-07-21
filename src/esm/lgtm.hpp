#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {


struct LightingTemplateData {
	rgba ambientColor;
	rgba directionalColor;
	rgba fogColor;
	float fogNear;
	float fogFar;
	int32_t directionalRotationXY;
	int32_t directionalRotationZ;
	float directionalFade;
	float fogClipDistance;
	float fogPower;
};


struct LightingTemplate : public Record {

    LightingTemplateData data;

    LightingTemplate(ESMReader& reader);
};
};