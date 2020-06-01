#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

class ESMReader;

struct CaravanMoney : public Record {
    std::string  editorId;
    ObjectBounds objectBounds;
    std::string  name;
    ModelData    modelData;
    std::string  largeIconFilename;
    std::string  smallIconFilename;
    formid       pickUpSound;
    formid       dropSound;
    uint32_t     absoluteValue;

    CaravanMoney(ESMReader& reader);
};
}