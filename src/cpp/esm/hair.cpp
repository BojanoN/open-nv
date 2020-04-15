#include "hdpt.hpp"

namespace ESM {

void Hair::load(ESMReader& reader) {
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
	reader.checkSubrecordHeader(ESMType.ICON);
	reader.readArraySubrecord<char>(texture.c_str());

	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType.DATA);
	reader.readSubrecord<uint8_t>(flags);
};