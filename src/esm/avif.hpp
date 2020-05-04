#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct ActorValueInformation : public Record {

    std::string   editorId;
    std::string   name;
    std::string	  description;
    std::string	  largeIconFilename;
    std::string	  smallIconFilename;
    std::string	  shortName;

    ActorValueInformation(ESMReader& reader);
};
};