/*#include "gmst.hpp"

namespace ESM {

void GameSetting::load(ESMReader& reader) {
	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESM::Names.EDID);
	reader.readArraySubrecord<char>(editorId.c_str());

	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESM::Names.FNAM);
	reader.readSubrecord<uint8_t>(type);

	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESM::Names.FLTV);
	switch(type) {
		case 's':
			value = reader.readSubrecord<int16_t>(value);
			break;
		case 'f':
			value = reader.readSubrecord<uint32_t>(value);
			break;
		default:
			value = reader.readSubrecord<float>(value);
			break;
	}
}

};*/