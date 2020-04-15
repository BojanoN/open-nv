/*#include "gmst.hpp"

namespace ESM {

void GameSetting::load(ESMReader& reader) {
	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESM::Names.EDID);
	reader.readArraySubrecord<char>(editorId.c_str());

	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESM::Names.DATA);
	switch(editorId[0]) {
		case 's':
			value = reader.readArraySubrecord<char>(value.c_str());
			break;
		case 'f':
			value = reader.readSubrecord<float>(value);
			break;
		default:
			value = reader.readSubrecord<int32_t>(value);
			break;
	}
}

};*/