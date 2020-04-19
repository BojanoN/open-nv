#include "gmst.hpp"

namespace ESM {

GameSetting::GameSetting(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType::EDID);
	reader.readStringSubrecord(editorId);

	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType::DATA);
	switch(editorId[0]) {
		case 's':
			value.emplace<2>();
			reader.readStringSubrecord(std::get<std::string>(value));
			break;
		case 'f':
			value.emplace<1>();
			reader.readSubrecord(std::get<float>(value));
			break;
		default:
			//default type of value is uint32_t
			reader.readSubrecord(std::get<int32_t>(value));
			break;
	}
}

};