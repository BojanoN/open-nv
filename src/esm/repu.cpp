#include "repu.hpp"

namespace ESM {
Reputation::Reputation(ESMReader& reader)
    : Record(reader)
{

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::FULL:
            reader.readStringSubrecord(name);
            break;
        case ESMType::ICON:
            reader.readStringSubrecord(largeIconFilename);
            break;
        case ESMType::MICO:
            reader.readStringSubrecord(smallIconFilename);
            break;
        case ESMType::DATA:
            reader.readSubrecord(value);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
};