#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {


struct MusicType : public Record {

    std::string  editorId;
    std::string  filename;
    float 		 db; //music loops if positive

    MusicType(ESMReader& reader);
};
};