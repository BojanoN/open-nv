#include "clas.hpp"
#include "reader.hpp"
#include "record.hpp"

namespace ESM {

void Class::load(ESMReader& reader)
{
    /*
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readArraySubrecord<char>(editorId.c_str());

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::FULL);
    reader.readArraySubrecord<char>(name.c_str());

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DESC);
    reader.readArraySubrecord<char>(description.c_str());

    while (reader.subrecordType().intValue != ESMType::DATA) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType().intValue) {
        case ESMType::ICON:
            reader.readArraySubrecord<char>(largeIconFilename);
            break;
        case ESMType::MICO:
            reader.readArraySubrecord<char>(smallIconFilename);
            break;
        default:
            break;
        }
    }

    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord<ClassData>(classData);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::ATTR);
    reader.readSubrecord<Attributes>(attributes);
    */
}

};
