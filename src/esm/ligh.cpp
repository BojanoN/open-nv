#include "ligh.hpp"
#include <cstdlib>

namespace ESM {

Light::Light(ESMReader& reader) : Record(reader)
{
    if(sizeof(LightData) != 32) {
        std::cout <<"kuracaaaaa";
        std::cout << 5 * sizeof(uint32_t) + 3 * sizeof(float);
        std::exit(1);
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    std::unordered_set<ESMType> nexts{ESMType::ICON, ESMType::MICO, ESMType::SCRI, ESMType::FULL, ESMType::DATA};

    while(reader.peekNextType() != ESMType::DATA) {
    	reader.readNextSubrecordHeader();
    	switch(reader.subrecordType()) {
            case ESMType::OBND:
                reader.readSubrecord(objectBounds);
                break;
            case ESMType::MODL:
                ModelData::load(reader, modelData, 0);
                break;
            case ESMType::SCRI:
                reader.readSubrecord(script);
                break;
    		case ESMType::FULL:
    			reader.readStringSubrecord(name);
    			break;
            case ESMType::ICON:
                reader.readStringSubrecord(largeIconFilename);
                break;
            case ESMType::MICO:
                reader.readStringSubrecord(smallIconFilename);
                break;
    		default:
    			std::stringstream s;
    			s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
    			reader.reportError(s.str()); 
    	}
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(lightData);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FNAM);
    reader.readSubrecord(fadeValue);

    if(reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        reader.checkSubrecordHeader(ESMType::SNAM);
        reader.readSubrecord(sound);
    }
}

};
