#pragma once
#include "reader.hpp"
#include "record.hpp"
#include <variant>

namespace ESM {

struct GameSetting : public Record {
    
    std::variant<int32_t, float, std::string> value;

    GameSetting(ESMReader& reader);
};

};