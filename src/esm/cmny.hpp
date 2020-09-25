#pragma once
#include "record.hpp"
#include "subrecords/objectbounds.hpp"
#include "subrecords/modeldata.hpp"
#include "structs.hpp"

namespace ESM {

class ESMReader;

struct CaravanMoney : public Record {
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
