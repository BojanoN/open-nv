#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "subrecords/modeldata.hpp"
#include "subrecords/destructiondata.hpp"
#include "subrecords/skill_common.hpp"
#include "structs.hpp"

namespace ESM {
class ESMReader;

enum class BookFlag : uint8_t {
    UNKNOWN   = 0x01,
    CANT_TAKE = 0x02
};

struct __attribute__((packed)) BookData {
    BookFlag flag;
    Skills   skill;
    int32_t  value;
    float    weight;
};

struct Book : public Record {
    ObjectBounds    objectBounds;
    std::string     name;
    ModelData       modelData;
    std::string     largeIconFilename;
    std::string     smallIconFilename;
    formid          script;
    std::string     description;
    DestructionData destructionData;
    formid          pickUpSound;
    formid          dropSound;
    BookData        data;

    Book(ESMReader& reader);
};
}
