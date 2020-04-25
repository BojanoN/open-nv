#include "idlm.hpp"
#include "reader.hpp"

namespace ESM {

IdleMarker::IdleMarker(ESMReader& reader)
    : Record(reader)
{
    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();

        switch (reader.subrecordType()) {
        case (ESMType::EDID):
            reader.readStringSubrecord(this->editorId);
            break;
        case (ESMType::OBND):
            reader.readSubrecord(this->objectBounds);
            break;
        case (ESMType::IDLF):
            reader.readSubrecord(this->flags);
            break;
        case (ESMType::IDLC): {
            reader.readRawData(this->data);
            if (reader.getCurrentSubrecord().dataSize != 1) {
                reader.skipSubrecord();
            }
            break;
        }
        case (ESMType::IDLT):
            reader.readSubrecord(this->idleTimer);
            break;
        case (ESMType::IDLA):
            reader.readArraySubrecord(this->animations);
            break;
        }
    }
}
}
