#include "navm.hpp"
#include "reader.hpp"

namespace ESM {

void NavigationMeshGrid::load(ESMReader& reader, std::vector<NavigationMeshGrid>& vector)
{
    vector.emplace_back();
    NavigationMeshGrid& tmp      = vector.back();
    uint32_t            cellSize = (reader.getCurrentSubrecord().dataSize - 36);

    reader.readRawData(tmp.coords);
    tmp.cells.resize(cellSize);
    reader.readRawArray(tmp.cells.data(), cellSize);
}

NavigationMesh::NavigationMesh(ESMReader& reader)
    : Record(reader)
{

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::EDID:
            reader.readStringSubrecord(this->editorId);
            break;
        case ESMType::NVER:
            reader.readSubrecord(this->version);
            break;
        case ESMType::DATA:
            reader.readSubrecord(this->data);
            break;
        case ESMType::NVVX:
            reader.readArraySubrecord(this->vertices);
            break;
        case ESMType::NVTR:
            reader.readArraySubrecord(this->triangles);
            break;
        case ESMType::NVCA:
            reader.skipSubrecord();
            break;
        case ESMType::NVDP:
            reader.readArraySubrecord(this->doors);
            break;
        case ESMType::NVGD:
            NavigationMeshGrid::load(reader, this->meshGrids);
            break;
        case ESMType::NVEX:
            reader.readArraySubrecord(this->externalConnections);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
