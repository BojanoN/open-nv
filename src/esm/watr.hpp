#pragma once
#include "record.hpp"
#include "reader.hpp"
#include "structs.hpp"

namespace ESM {

enum class WaterFlags : uint8_t {
    CAUSES_DAMAGE  = 0x01,
    REFLECTIVE     = 0x02
};

struct WaterVisualData {
	uint8_t unknown[16];
	
	float sunPower;
	float reflectivityAmount;
	float fresnelAmount;
	uint8_t unused_1[4];
	
	float aboveWaterFognearPlaneDistance;
	float aboveWaterFogfarPlaneDistance;
	
	rgba    shallowColor;
	rgba    deepColor;
	rgba    reflectionColor;
	uint8_t unused_2[4];
	
	float rainForce;
	float rainBelocity;
	float rainFalloff;
	float rainDampener;
	
	float displacementStartingSize;
	float displacementForce;
	float displacementVelocity;
	float displacementFalloff;
	float displacementDampener;
	
	float rainStartingSize;
	
	float noiseScale;
	
	float noiseLayer1WindDirection;
	float noiseLayer2WindDirection;
	float noiseLayer3WindDirection;
	
	float noiseLayer1WindSpeed;
	float noiseLayer2WindSpeed;
	float noiseLayer3WindSpeed;
	
	float noiseDepthFallofStart;
	float noiseDepthFallofEnd;
	
	float aboveWaterFogAmount;

	float noiseNormalsUvScale;
	
	float underWaterfogAmount;
	float underWaternearPlaneDistance;
	float underWaterfarPlaneDistance;
	
	float distortionAmount;
	float shininess;
	float reflectionHdrMultiplier;
	float lightRadius;
	float lightBrightness;

	float noiseLayer1UvScale;
	float noiseLayer2UvScale;
	float noiseLayer3UvScale;

	float noiseLayer1AmplitudeScale;
	float noiseLayer2AmplitudeScale;
	float noiseLayer3AmplitudeScale;
};

struct RelatedWaters {
	formid daytime;
	formid nighttime;
	formid underwater;
};

struct Water : public Record {
    std::string     name;
    std::string     noiseMap;
    uint8_t			opacity;
    WaterFlags		flags;
    std::string 	materialId;
    formid 			sound; //SOUN
    formid 			actorEffect; //SPEL
    uint16_t		damage;
    WaterVisualData visualData;
    RelatedWaters   relatedWaters;

    Water(ESMReader& reader);
};

}