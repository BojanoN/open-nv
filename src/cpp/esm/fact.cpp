#include "fact.hpp"

namespace ESM {

void Faction::loadRankCollection(ESMReader& reader)
{
    while (reader.hasMoreSubrecords() && reader.subrecordType() != ESMType::WMI1) {

        switch (reader.subrecordType()) {
        case ESMType::RNAM:
            ranks.emplace_back();
            reader.readSubrecord(ranks.back().rankNumber);
            break;
        case ESMType::MNAM:
            reader.readStringSubrecord(ranks.back().male);
            break;
        case ESMType::FNAM:
            reader.readStringSubrecord(ranks.back().female);
            break;
        case ESMType::INAM:
            reader.readSubrecord(ranks.back().insignia);
            break;
        }

        reader.readNextSubrecordHeader();
    }
    reader.rewind(sizeof(SubrecordHeader));
}

Faction::Faction(ESMReader& reader)
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
        case ESMType::XNAM:
            relations.emplace_back();
            reader.readSubrecord(relations.back());
            break;
        case ESMType::DATA: {
            if (reader.getCurrentSubrecord().dataSize == 1) {
                reader.readSubrecord(factionData.flags1);
            } else {
                reader.readSubrecord(factionData);
            }
            break;
        }
        case ESMType::CNAM:
            reader.readSubrecord(unused);
            break;
        case ESMType::RNAM:
            loadRankCollection(reader);
            break;
        case ESMType::WMI1:
            reader.readSubrecord(reputation);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType());
            log_fatal(s.str().c_str());
            reader.reportError(s.str());
        }
    }
}

};
