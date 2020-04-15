#include "hdpt.hpp"

namespace ESM {

void HeadPart::load(ESMReader& reader) {
	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType.EDID);
	reader.readArraySubrecord<char>(editorId.c_str());

	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType.FULL);
	reader.readArraySubrecord<char>(name.c_str());

	if(reader.peekNextType().intValue == ESMType.MODL) {
		ModelData.load(reader, modelData, 0, ESMType.DATA);
	}

	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType.DATA);
	reader.readSubrecord<uint8_t>(flags);

	while(reader.hasMoreSubrecords()) {
		reader.readNextSubrecordHeader();
		reader.checkSubrecordHeader(ESMType.HNAM);
		extraParts.push_back(0);
		reader.readSubrecord<formid>(extraParts.back());
	}

};