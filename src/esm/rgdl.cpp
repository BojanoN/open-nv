#include "rgdl.hpp"

namespace ESM {

Ragdoll::Ragdoll(ESMReader& reader) : Record(reader) {
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::NVER);
    reader.readSubrecord(version);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(data);
    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::XNAM) {
    	reader.readSubrecord(actorBase);
	    reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::TNAM);
    reader.readSubrecord(bodyPart);
    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::RAFD) {
    	reader.readSubrecord(feedbackData);
    	reader.readNextSubrecordHeader();
    }
    if(reader.subrecordType() == ESMType::RAFB) {
    	reader.readArraySubrecord(feedbackDynamicBones);
    	reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::RAPS);
    reader.readSubrecord(poseMatchingData);

    if(reader.hasMoreSubrecords()) {
    	reader.readNextSubrecordHeader();
    	reader.checkSubrecordHeader(ESMType::ANAM);
    	reader.readStringSubrecord(deathPose);
    }
}

};