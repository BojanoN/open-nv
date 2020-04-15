/*#include "hdpt.hpp"

namespace ESM {

void Hair::load(ESMReader& reader) {
	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESM::Names.EDID);
	reader.readArraySubrecord<char>(editorId.c_str());

	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESM::Names.FULL);
	reader.readArraySubrecord<char>(name.c_str());

	if(reader.peekNextType().intValue == ESM::Names.MODL) {
		ModelData.load(reader, modelData, 0, ESM::Names.DATA);
	}

	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESM::Names.ICON);
	reader.readArraySubrecord<char>(texture.c_str());

	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESM::Names.DATA);
	reader.readSubrecord<uint8_t>(flags);
};*/