#include "gmst.hpp"

namespace ESM {

void GameSetting::load(ESMReader& reader) {
	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType::EDID);
	reader.readStringSubrecord(editorId);

	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType::DATA);
	/*switch(editorId[0]) {
		case 's':
			value = "";
			reader.readStringSubrecord(value);
			break;
		case 'f':
			value = 0.f;
			reader.readSubrecord(value);
			break;
		default:
			value = 0;
			reader.readSubrecord(value);
			break;
	}*/
}

};