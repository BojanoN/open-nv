#pragma once
#include "record.hpp"
#include "subrecords/scriptdata.hpp"
#include "structs.hpp"
#include "types.hpp"
#include <variant>

namespace ESM {
class ESMReader;

struct Script : public Record {
    ScriptData data;

    Script(ESMReader& reader);
};
}
