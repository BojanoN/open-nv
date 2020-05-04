#include "lgtm.hpp"


namespace ESM {

LightingTemplate::LightingTemplate(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(data);
}

};