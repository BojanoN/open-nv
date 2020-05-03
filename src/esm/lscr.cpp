#include "lscr.hpp"
#include "reader.hpp"

namespace ESM {

LoadingScreen::LoadingScreen(ESMReader& reader)
    : Record(reader)
{

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::EDID:
            reader.readStringSubrecord(this->editorId);
            break;
        case ESMType::ICON:
            reader.readStringSubrecord(this->largeIconFilename);
            break;
        case ESMType::MICO:
            reader.readStringSubrecord(this->smallIconFilename);
            break;
        case ESMType::DESC:
            reader.readStringSubrecord(this->description);
            break;
        case ESMType::LNAM:
            reader.readArraySubrecord(this->locations);
            break;
        case ESMType::WMI1:
            reader.readSubrecord(this->loadScreenType);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
