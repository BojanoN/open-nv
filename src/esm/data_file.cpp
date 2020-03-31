#include "data_file.hpp"
#include "records.hpp"


Record* Record::ParseRecord(std::ifstream& stream, uint32_t type)
{
  if (Record::prototypes.count(type))
    return Record::prototypes[type]->clone(stream);
  else
    throw std::runtime_error("Type not implemented");
}

std::unordered_map<uint32_t, Record*> Record::prototypes({{0xdeadbeef, new TES4Record()}});
