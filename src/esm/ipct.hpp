#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {


enum class ImpactEffectOrientation : uint32_t {
	SurfaceNormal,
	ProjectileVector,
	ProjectileReflection
};

enum class ImpactFlags : uint32_t {
	NO_DECAL_DATA = 0x00000001
};
 
struct ImpactData {
	float effectDuration;
	ImpactEffectOrientation effectOrientation;
	float angleThreshold;
	float placementRadius;
	SoundLevel soundLevel;
	ImpactFlags  flags;
};

struct Impact : public Record {

    std::string  editorId;
    ModelData	 modelData;
    ImpactData   data;
    DecalData    decalData;
    formid 		 textureSet; //TXST
    formid  	 sound_1; //SOUN
    formid  	 sound_2; //SOUN

    Impact(ESMReader& reader);
};
};