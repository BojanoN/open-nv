#include "data_file.hpp"
#include "records.hpp"

Record* Record::ParseRecord(std::ifstream& stream, uint32_t type)
{
    return nullptr;
}

std::unordered_map<uint32_t, Record> Record::prototypes = {{0xdeadbeef, TES4Record()}};
