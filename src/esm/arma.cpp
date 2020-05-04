#include "arma.hpp"

namespace ESM {

ArmorAddon::ArmorAddon(ESMReader& reader) : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::OBND);
    reader.readSubrecord(objectBounds);
    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::FULL) {
        reader.readStringSubrecord(name);
        reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::BMDT);
    reader.readSubrecord(bipedData);

    while(reader.peekNextType() != ESMType::ETYP) {
    	reader.readNextSubrecordHeader();
    	switch(reader.subrecordType()) {
    		case ESMType::MODL:
    			ModelData::load(reader, maleBipedModelData, 0);
    			break;
    		case ESMType::MOD2:
    			ModelData::load(reader, maleWorldModelData, 1);
    			break;
    		case ESMType::ICON:
    			reader.readStringSubrecord(maleInventoryIconFilename);
    			break;
    		case ESMType::MICO:
    			reader.readStringSubrecord(maleMessageIconFilename);
    			break;
    		case ESMType::MOD3:
    			ModelData::load(reader, femaleBipedModelData, 2);
    			break;
    		case ESMType::MOD4: //Female world model missing when same as male model?
			    ModelData::load(reader, femaleWorldModelData, 3);
			    break;
    		case ESMType::ICO2:
    			reader.readStringSubrecord(femaleInventoryIconFilename);
    			break;
    		case ESMType::MIC2:
    			reader.readStringSubrecord(femaleMessageIconFilename);
    			break;
    		case ESMType::MOSD: //can be placed outside of MOD3 collection
    			reader.readSubrecord(femaleBipedModelData.FaceGenModelFlags);
    			break;
    		default:
    			std::stringstream s;
    			s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
    			reader.reportError(s.str()); 
    	}
    }
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::ETYP);
    reader.readSubrecord(equipmentType);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(data);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DNAM);
    ArmorAttributes::load(reader, attributes);    
}

};