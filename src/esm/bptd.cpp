#include "bptd.hpp"

namespace ESM {

BodyPartData::BodyPartData(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::MODL);
    ModelData::load(reader, modelData, 0);

    while(reader.peekNextType() == ESMType::BPTN ||
    		reader.peekNextType() == ESMType::BPNN) {
    	nodes.emplace_back();
    	BodyPartNode::load(reader, nodes.back());
    }
    if(reader.hasMoreSubrecords()) {
    	reader.readNextSubrecordHeader();
    	reader.checkSubrecordHeader(ESMType::RAGA);
    	reader.readSubrecord(ragdoll);
    }
}

void BodyPartNode::load(ESMReader& reader, BodyPartNode& node) {
    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::BPTN) {
        reader.readStringSubrecord(node.name);
        reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::BPNN);
    reader.readStringSubrecord(node.nodeName);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::BPNT);
    reader.readStringSubrecord(node.VATSTarget);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::BPNI);
    reader.readStringSubrecord(node.ikDataStartNode);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::BPND);
    reader.readSubrecord(node.data);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::NAM1);
    reader.readStringSubrecord(node.limbReplacementModel);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::NAM4);
    reader.readStringSubrecord(node.goreEffectsTargetBone);

    if(reader.peekNextType() == ESMType::NAM5) {
    	reader.readNextSubrecordHeader();
        reader.readArraySubrecord(node.textureFileHashes);
    }
}

};