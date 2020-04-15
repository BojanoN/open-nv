#include "hdpt.hpp"

namespace ESM {

void Eyes::load(ESMReader& reader) {
	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType.EDID);
	reader.readArraySubrecord<char>(editorId.c_str());

	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType.FULL);
	reader.readArraySubrecord<char>(name.c_str());

	reader.readNextSubrecordHeader();
	if(reader.subrecordType().intValue == ESMType.ICON) {
		reader.readArraySubrecord<char>(texture.c_str());
		reader.readNextSubrecordHeader();
	}
	
	reader.checkSubrecordHeader(ESMType.DATA);
	reader.readSubrecord<uint8_t>(flags);
};