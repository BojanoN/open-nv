#pragma once
#include "record.hpp"
#include "structs.hpp"

#include <vector>

namespace ESM {

class ESMReader;

struct LoadingScreenLocation {
    formid  direct; // CELL or WRLD
    formid  indirectWorld; // WRLD
    int16_t indirectGridY;
    int16_t indirectGridX;
};

struct LoadingScreen : public Record {
    std::string                        editorId;
    std::string                        largeIconFilename;
    std::string                        smallIconFilename;
    std::string                        description;
    std::vector<LoadingScreenLocation> locations;
    formid                             loadScreenType;

    LoadingScreen(ESMReader& reader);
};

}
