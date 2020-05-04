#include "addn.hpp"

namespace ESM {

AddonNode::AddonNode(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::OBND);
    reader.readSubrecord(objectBounds);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::MODL);
    ModelData::load(reader, modelData, 0);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(nodeIndex);
    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::SNAM) {
    	reader.readSubrecord(unknown);
    	reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::DNAM);
    reader.readSubrecord(masterParticleSystemCap);
} 

};