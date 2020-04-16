#include "clas.hpp"

namespace ESM {

Class::Class(ESMReader& reader) : Record(reader)
{
    
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FULL);
    reader.readStringSubrecord(name);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DESC);
    reader.readStringSubrecord(description);

    while (reader.subrecordType() != ESMType::DATA) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::ICON:
            reader.readStringSubrecord(largeIconFilename);
            break;
        case ESMType::MICO:
            reader.readStringSubrecord(smallIconFilename);
            break;
        default:
            break;
        }
    }

    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(classData);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::ATTR);
    reader.readSubrecord(attributes);
    
}

};
