#include "idle.hpp"


namespace ESM {

void IdleAnimationData::load(ESMReader& reader, IdleAnimationData& data)
{
    if (reader.getCurrentSubrecord().dataSize == 6) {
        reader.readSubrecord(*reinterpret_cast<IdleAnimationData::IdleAnimationDataSmall*>(&data));
    } else {
        reader.readSubrecord(data);
    }
}

IdleAnimation::IdleAnimation(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
	if(reader.subrecordType() == ESMType::EDID) {
    	reader.readStringSubrecord(editorId);
    	reader.readNextSubrecordHeader();
    } 
    reader.checkSubrecordHeader(ESMType::MODL);
    ModelData::load(reader, modelData, 0);

    while(reader.peekNextType() == ESMType::CTDA) {
    	reader.readNextSubrecordHeader();
    	conditions.emplace_back();
    	Condition::load(reader, conditions.back());
    }
    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::ANAM) {
    	reader.readSubrecord(relatedIdleAnimations);
    	reader.readNextSubrecordHeader();
    } 
    reader.checkSubrecordHeader(ESMType::DATA);
    IdleAnimationData::load(reader, data);
}

};