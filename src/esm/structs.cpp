#include "structs.hpp"
#include "util/strmanip.hpp"

namespace ESM {

void Condition::load(ESMReader& reader, Condition& condition)
{
    reader.readRawDataSubrecSize(condition);
}


};
