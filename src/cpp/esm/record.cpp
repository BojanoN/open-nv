#include "record.hpp"
#include "reader.hpp"

using namespace ESM;

Record::Record(ESMReader& reader)
{
    reader.readNextRecord();
    RecordHeader& hdr = reader.getCurrentRecord();

    this->Flags = hdr.Flags;
    this->ID    = hdr.ID;
};
