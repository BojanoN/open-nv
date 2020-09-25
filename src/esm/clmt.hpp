#pragma once
#include "record.hpp"
#include "subrecords/modeldata.hpp"
#include "structs.hpp"

namespace ESM {

struct WeatherTypes {
    formid  weather; // WTHR
    int32_t chance;
    formid  global; // GLOB
};

struct __attribute__((packed)) ClimateTiming {
    uint8_t sunriseBegin;
    uint8_t sunriseEnd;
    uint8_t sunsetBegin;
    uint8_t sunsetEnd;
    uint8_t volatility;
    uint8_t moons;
};

struct Climate : public Record {
    std::vector<WeatherTypes> weatherTypes;
    std::string               sunTexture;
    std::string               sunGlareTexture;
    ModelData                 modelData;
    ClimateTiming             timing;

    Climate(ESMReader& reader);
};

}
