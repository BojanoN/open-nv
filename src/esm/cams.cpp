#include "cams.hpp"


namespace ESM {

CameraShot::CameraShot(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::MODL) {
        ModelData::load(reader, modelData, 0);
    	reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::DATA);
    //reader.readSubrecord(data);
    reader.readRawDataSubrecSize(data);
    if(reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        reader.checkSubrecordHeader(ESMType::MNAM);
        reader.readSubrecord(imageSpaceModifier);
    }
}

};