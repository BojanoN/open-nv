#include "cpth.hpp"


namespace ESM {

CameraPath::CameraPath(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
    while(reader.peekNextType() == ESMType::CTDA) {
        reader.readNextSubrecordHeader();
        conditions.emplace_back();
        Condition::load(reader, conditions.back());
    }
    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::ANAM) {
        reader.readSubrecord(relatedCameraPaths);
    	reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(zoom);
    while(reader.peekNextType() == ESMType::SNAM) {
        reader.readNextSubrecordHeader();
        cameraShots.emplace_back();
        reader.readSubrecord(cameraShots.back());
    }
}

};