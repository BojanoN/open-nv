#pragma once
#include "record.hpp"
#include "types.hpp"
#include "structs.hpp"
#include <variant>

namespace ESM {
class ESMReader;

struct Script : public Record {
    std::string                  editorId;
    ScriptData                   data;

    Script(ESMReader& reader);
};
}
