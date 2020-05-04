#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {


struct DefaultObjects {
	formid stimpack;
	formid superStimpack;
	formid radX;
	formid radAway;
	formid morphine;
	formid perkParalysis;
	formid playerFaction;
	formid mysteriousStrangerNpc;
	formid mysteriousStrangerFaction;
	formid defaultMusic;
	formid battleMusic;
	formid deathMusic;
	formid successMusic;
	formid levelUpMusic;
	formid playerVoiceMale;
	formid playerVoiceMaleChild;
	formid playerVoiceFemale;
	formid playerVoiceFemaleChild;
	formid eatPackageDefaultFood;
	formid everyActorAbility;
	formid drugWearsOffImageSpace;
	formid doctorsBag;
	formid missFortuneNpc;
	formid missFortuneFaction;
	formid meltdownExplosion;
	formid unarmedForwardPa;
	formid unarmedBackwardPa;
	formid unarmedLeftPa;
	formid unarmedRightPa;
	formid unarmedCrouchPa;
	formid unarmedCounterPa;
	formid spotterEffect;
	formid itemDetectedEffect;
	formid cateyeMobileEffect;
};

struct DefaultObjectManager : public Record {

    std::string  editorId;
    DefaultObjects objects;

    DefaultObjectManager(ESMReader& reader);
};
};