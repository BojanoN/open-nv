#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct Reputation : public Record {
    std::string     editorId;
    std::string     name;
    std::string     largeIconFilename;
    std::string     smallIconFilename;
    float           value;
    
    Reputation(ESMReader& reader);
};
}