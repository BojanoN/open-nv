#include "esm.hpp"
#include "tes4.hpp"

namespace ESM {

ESM::ESM(std::string& path)
    : reader(path)
    , records()
{
    while (this->reader.hasMoreRecords()) {
        Record* rec            = this->reader.nextRecord();
        this->records[rec->ID] = rec;
    }
};

ESM::~ESM() {};

Record* ESMReader::nextRecord()
{
    TypeRecordLoader<TES4>* rl = new TypeRecordLoader<TES4>();
    RecordLoader*           ret;

    ESMReader::recordLoaders["TES4"] = rl;
    ret                              = ESMReader::recordLoaders["TES4"];
    (*ret)(this->file);
    Record* tmp = nullptr;
    return tmp;
}

std::unordered_map<std::string, RecordLoader*> ESMReader::recordLoaders;
};
