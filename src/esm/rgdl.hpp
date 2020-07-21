#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {


struct RagdollData {
	uint32_t dynamicBoneCount;
	uint32_t padding;
	uint8_t	 feedbackEnabled; //1 - yes, 0 - no
	uint8_t	 footIkEnabled;
	uint8_t	 lookIkEnabled;
	uint8_t	 grabIkEnabled;
	uint8_t	 poseMatching;
	uint8_t  unused;
};

struct RagdollFeedbackData {
	float dynamicKeyframeBlendAmount;
	float hierarchyGain;
	float positionGain;
	float velocityGain;
	float accelerationGain;
	float snapGain;
	float velocityDamping;
	float snapMaxLinearVelocity;
	float snapMaxAngularVelocity;
	float snapMaxLinearDistance;
	float snapMaxAngularDistance;
	float positionMaxLinearVelocity;
	float positionMaxAngularVelocity;
	int32_t projectilePositionMaxVelocity; //divide by 1000
	uint32_t meleePositionMaxVelocity; //divide by 1000
};

enum class RagdollPoseMatchingFlags : uint8_t {
	DISABLE_ON_MOVE = 0x01
};

struct RagdollPoseMatchingData {
	uint16_t matchBone_1;
	uint16_t matchBone_2;
	uint16_t matchBone_3;
	RagdollPoseMatchingFlags flags;
	uint8_t unused;
	float motorsStrength;
	float poseActivationDelayTime;
	float matchErrorAllowance;
	float displacementToDisable;
};

struct Ragdoll : public Record {

    uint32_t version;
    RagdollData data;
    formid  actorBase; //CREA, NPC_
    formid  bodyPart; //BPTD
    RagdollFeedbackData feedbackData;
    std::vector<uint16_t> feedbackDynamicBones;
    RagdollPoseMatchingData poseMatchingData;
    std::string deathPose;

    Ragdoll(ESMReader& reader);
};
};