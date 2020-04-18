#include "ench.hpp"

namespace ESM {

ObjectEffect::ObjectEffect(ESMReader& reader) : Record(reader) {
	
	reader.readNextSubrecordHeader();
	reader.checkSubrecordHeader(ESMType::EDID);
	reader.readStringSubrecord(editorId);

	reader.readNextSubrecordHeader();
	if(reader.subrecordType() == ESMType::FULL) {
		reader.readStringSubrecord(name);
		reader.readNextSubrecordHeader();
	}

	reader.checkSubrecordHeader(ESMType::ENIT);
	reader.readSubrecord(data);

	if(!reader.hasMoreSubrecords()) {
		reader.reportError("At least one effect must be specified for an ENCH record.");
	}
	while(reader.hasMoreSubrecords()) {
		reader.readNextSubrecordHeader();
		reader.checkSubrecordHeader(ESMType::EFID);
		effects.emplace_back();
		reader.readSubrecord(effects.back().baseEffect);

		reader.readNextSubrecordHeader();
		reader.checkSubrecordHeader(ESMType::EFIT);
		reader.readSubrecord(effects.back().data);

		while(reader.peekNextType() == ESMType::CTDA) {
			reader.readNextSubrecordHeader();
			Condition::load(reader, effects.back().condition);
		}
	}
}

};