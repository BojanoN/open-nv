#include "reader.hpp"
#include "record.hpp"

namespace ESM {

struct GameSetting : public Record {
    
    std::string editorId;
    std::variant<int32_t, float, std::string> value;

    virtual void load(ESMReader& reader);
}

};