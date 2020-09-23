#include "pack.hpp"

namespace ESM {


Package::Package(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::PKDT);
    //reader.readSubrecord(data);
    reader.readRawDataSubrecSize(data);

    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::PLDT) {
    	reader.readSubrecord(location_1);
    	reader.readNextSubrecordHeader();
    }
    if(reader.subrecordType() == ESMType::PLD2) {
    	reader.readSubrecord(location_2);
    	reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::PSDT);
    reader.readSubrecord(schedule);



    while(reader.hasMoreSubrecords()) {
    	reader.readNextSubrecordHeader();
    	switch(reader.subrecordType()) {
    		case ESMType::PTDT:
    			reader.readSubrecord(target_1);
    			break;
    		case ESMType::CTDA:
    			conditions.emplace_back();
    			Condition::load(reader, conditions.back());
    			break;
    		case ESMType::IDLF:
    			reader.readSubrecord(idleAnimationFlags);
    			break;
    		case ESMType::IDLC:
    			//reader.readSubrecord(idleAnimationCount);
    			reader.readRawDataSubrecSize(idleAnimationCount);
                break;
    		case ESMType::IDLT:
    			reader.readSubrecord(idleTimerSetting);
    			break;
    		case ESMType::IDLA:
    			reader.readArraySubrecord(idleAnimations);
    			break;
    		case ESMType::IDLB:
    			reader.readSubrecord(unused);
    			break;
    		case ESMType::CNAM:
    			reader.readSubrecord(combatStyle);
    			break;
    		case ESMType::PKED:
    			eat = true;
    			break;
    		case ESMType::PKE2:
    			reader.readSubrecord(escortDistance);
    			break;
    		case ESMType::PKFD:
    			reader.readSubrecord(followTriggerRadius);
    			break;
    		case ESMType::PKPT:
                reader.readRawDataSubrecSize(patrolFlags);
    			//reader.readSubrecord(patrolFlags);
    			break;
    		case ESMType::PKW3:
    			reader.readSubrecord(useWeaponData);
    			break;
    		case ESMType::PTD2:
    			reader.readSubrecord(target_2);
    			break;
    		case ESMType::PUID:
    			useItem = true;
    			break;
    		case ESMType::PKAM:
    			ambush = true;
    			break;
    		case ESMType::PKDD:
    			reader.readSubrecord(dialogue);
    			break;
    		case ESMType::PLD2:
    			reader.readSubrecord(location_2);
    			break;
    		case ESMType::POBA:
    			loadIdleScriptTopicGroup(reader, onBeginIdle, onBeginScript, onBeginTopic);
    			break;
    		case ESMType::POEA:
    			loadIdleScriptTopicGroup(reader, onEndIdle, onEndScript, onEndTopic);
    			break;
    		case ESMType::POCA:
    			loadIdleScriptTopicGroup(reader, onChangeIdle, onChangeScript, onChangeTopic);
    			break;    			
        	default:
            	std::stringstream s;
            	s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            	reader.reportError(s.str());
        }
    }
}

void Package::loadIdleScriptTopicGroup(ESMReader& reader, formid& idle, ScriptData& script, formid& topic) {
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(INAM);
    reader.readSubrecord(idle);
    ScriptData::load(reader, script);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::TNAM);
    reader.readSubrecord(topic);
}

};