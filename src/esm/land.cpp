#include "land.hpp"
#include "reader.hpp"

namespace ESM {
Landscape::Landscape(ESMReader& reader)
    : Record(reader)
{

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::DATA:
            reader.skipSubrecord();
            break;
        case ESMType::VNML:
            this->vertexNormals.emplace_back();
            reader.readSubrecordTypeSize(this->vertexNormals.back());
            break;
        case ESMType::VCLR:
            this->vertexColors.emplace_back();
            reader.readSubrecordTypeSize(this->vertexColors.back());
            break;
        case ESMType::VHGT:
            reader.readSubrecordTypeSize(this->vertexHeightMap);
            break;
        case ESMType::VTEX:
            reader.readArraySubrecord(this->textures);
            break;
        case ESMType::BTXT:
            this->baseLayers.emplace_back();
            reader.readSubrecordTypeSize(this->baseLayers.back().header);
            break;
        case ESMType::ATXT:
            this->alphaLayers.emplace_back();
            reader.readSubrecordTypeSize(this->alphaLayers.back().header);
            break;
        case ESMType::VTXT:
            reader.readArraySubrecord(this->alphaLayers.back().data);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
