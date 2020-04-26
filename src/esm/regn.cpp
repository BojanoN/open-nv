#include "regn.hpp"
#include "reader.hpp"
#include <set>

static std::set<ESM::ESMType> regionDataCollection {
    ESM::ESMType::RDAT,
    ESM::ESMType::RDOT,
    ESM::ESMType::RDMP,
    ESM::ESMType::RDGS,
    ESM::ESMType::RDMD,
    ESM::ESMType::RDMO,
    ESM::ESMType::RDSI,
    ESM::ESMType::RDSD,
    ESM::ESMType::RDWT,
    ESM::ESMType::RDSB,
    ESM::ESMType::RDID
};

namespace ESM {

void RegionDataEntry::loadRegionData(ESMReader& reader, std::vector<RegionDataEntry>& regionData)
{
    while (regionDataCollection.count(reader.subrecordType())) {
        switch (reader.subrecordType()) {
        case ESMType::RDAT:
            regionData.emplace_back();
            reader.readSubrecord(regionData.back().header);
            break;
        case ESMType::RDOT:
            //   regionData.back().objects.emplace_back();
            reader.readArraySubrecord(regionData.back().objects);
            break;
        case ESMType::RDMP:
            reader.readStringSubrecord(regionData.back().mapName);
            break;
        case ESMType::RDGS:
            regionData.back().grasses.emplace_back();
            reader.readSubrecord(regionData.back().grasses.back());
            break;
        case ESMType::RDMD:
            reader.readSubrecord(regionData.back().musicType);
            break;
        case ESMType::RDMO:
            reader.readSubrecord(regionData.back().music);
            break;
        case ESMType::RDSI:
            reader.readSubrecord(regionData.back().mediaSet);
            break;
        case ESMType::RDSB:
            reader.readSubrecord(regionData.back().battleMediaSet);
            break;
        case ESMType::RDSD:
            reader.readArraySubrecord(regionData.back().sounds);
            break;
        case ESMType::RDWT:
            reader.readArraySubrecord(regionData.back().weatherTypes);
            break;
        case ESMType::RDID:
            reader.readArraySubrecord(regionData.back().imposters);
            break;
        default:
            break;
        }

        reader.readNextSubrecordHeader();
    }
    reader.rewind(sizeof(SubrecordHeader));
}

Region::Region(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();

        switch (reader.subrecordType()) {
        case ESMType::ICON:
            reader.readStringSubrecord(this->largeIconFilename);
            break;
        case ESMType::MICO:
            reader.readStringSubrecord(this->smallIconFilename);
            break;
        case ESMType::RCLR:
            reader.readSubrecord(this->mapColor);
            break;
        case ESMType::WNAM:
            reader.readSubrecord(this->worldspace);
            break;
        case ESMType::RDAT:
            RegionDataEntry::loadRegionData(reader, this->dataEntries);
            break;
        case ESMType::RPLI:
            this->areas.emplace_back();
            reader.readSubrecord(this->areas.back().edgeFallOff);
            break;
        case ESMType::RPLD:
            reader.readArraySubrecord(this->areas.back().data);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
