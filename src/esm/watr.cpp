#include "watr.hpp"

namespace ESM {

Water::Water(ESMReader& reader) : Record(reader) {

	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::FULL) {
        reader.readStringSubrecord(name);
        reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::NNAM);
    reader.readStringSubrecord(noiseMap);
    
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::ANAM);
    reader.readSubrecord(opacity);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FNAM);
    reader.readSubrecord(flags);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::MNAM);
    reader.readStringSubrecord(materialId);

    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::SNAM) {
        reader.readSubrecord(sound);
        reader.readNextSubrecordHeader();
    }
    if(reader.subrecordType() == ESMType::XNAM) {
        reader.readSubrecord(actorEffect);
        reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::DATA);
    if(uint32_t size = reader.getCurrentSubrecord().dataSize; size == sizeof(uint16_t)) {
        reader.readSubrecord(damage);
        reader.readNextSubrecordHeader();
        reader.checkSubrecordHeader(ESMType::DNAM);
        //reader.readSubrecord(visualData);
        reader.readRawDataSubrecSize(visualData);
    } else {
        reader.readDirect(reinterpret_cast<char*>(&visualData), size - sizeof(uint16_t));
        reader.readDirect(reinterpret_cast<char*>(&damage), sizeof(uint16_t));
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::GNAM);
    reader.readSubrecord(relatedWaters);
}

};