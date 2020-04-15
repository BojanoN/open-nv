#pragma once
#include "reader.hpp"
#include "record.hpp"
#include <variant>

namespace ESM {

struct GameSetting : public Record {
    
    std::string editorId;
    std::variant<std::string, float, int32_t> value;

    virtual void load(ESMReader& reader);
};

};