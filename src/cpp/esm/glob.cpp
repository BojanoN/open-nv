#include "glob.hpp"

namespace ESM {

GlobalVariable::GlobalVariable(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType::EDID);
	reader.readStringSubrecord(editorId);

	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType::FNAM);
	reader.readSubrecord(type);

	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType::FLTV);
	switch(type) {
		case 's':
			//value.emplace<1>();
			reader.readSubrecord(std::get<int32_t>(value));
			break;
		case 'f':
			value.emplace<2>();
			reader.readSubrecord(std::get<float>(value));
			break;
		case 'l':
			////default type of value is int32_t
			reader.readSubrecord(std::get<int32_t>(value));
			break;
		default:
			std::stringstream s;
			s << "Invalid global variable type " << type; 
			reader.reportError(s.str());
	}
}

};