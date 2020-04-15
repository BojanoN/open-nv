/*#include "eyes.hpp"

namespace ESM {

void Eyes::load(ESMReader& reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(Names.EDID);
    //reader.readArraySubrecord<char>(editorId.c_str());

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(Names.FULL);
    //reader.readArraySubrecord<char>(name.c_str());

    reader.readNextSubrecordHeader();
    if (reader.subrecordType().intValue == Names.ICON) {
        //reader.readArraySubrecord<char>(texture.c_str());
        reader.readNextSubrecordHeader();
    }

    reader.checkSubrecordHeader(Names.DATA);
    reader.readSubrecord(flags);
};
}*/
