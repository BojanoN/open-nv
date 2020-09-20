#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

enum class CameraShotAction : uint32_t {
	Shoot,
	Fly,
	Hit,
	Zoom
};

enum class CameraShotProperties : uint32_t {
	Attacker,
	Projectile,
	Target
};

enum class CameraShotFlags : uint32_t {
	POSITION_FOLLOWS_LOCATION = 0x00000001,
	ROTATION_FOLLOWS_TARGET = 0x00000002,
	DONT_FOLLOW_BONE = 0x00000004,
	FIRST_PERSON_CAMERA = 0x00000008,
	NO_TRACER = 0x00000010,
	START_AT_TIME_ZERO = 0x00000020
};

struct CameraShotData {
	CameraShotAction     action;
	CameraShotProperties location;
	CameraShotProperties target;
	CameraShotFlags      flags;
	float 				 playerTimeMultiplier;
	float 				 targetTimeMultiplier;
	float 				 globalTimeMultiplier;
	float 				 maxTime;
	float 				 minTime;
	float 				 targetPercentageBetweenActors = 0.f;
};

struct CameraShot : public Record {

    ModelData     modelData;
    CameraShotData data;
    formid 		   imageSpaceModifier; //IMAD

    CameraShot(ESMReader& reader);
};
};