#include "ipds.hpp"


namespace ESM {

ImpactDataset::ImpactDataset(ESMReader& reader) : Record(reader) {
	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(impacts);
}

};