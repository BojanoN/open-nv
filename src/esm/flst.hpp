#pragma once
#include "record.hpp"
#include "types.hpp"

namespace ESM {

class ESMReader;

struct FormIDList : public Record {
    std::vector<formid> formids;

    FormIDList(ESMReader& reader);
};
}
