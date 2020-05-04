#include "vtyp.hpp"


namespace ESM {

VoiceType::VoiceType(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
    if(reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        reader.checkSubrecordHeader(ESMType::DNAM);
        reader.readSubrecord(flags);
    }
}

};