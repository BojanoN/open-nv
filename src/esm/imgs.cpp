#include "imgs.hpp"

namespace ESM {

ImageSpace::ImageSpace(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DNAM);
    //reader.readSubrecord(data);
    reader.readRawDataSubrecSize(data);
}

};