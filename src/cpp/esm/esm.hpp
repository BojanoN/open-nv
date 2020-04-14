#pragma once

#include "reader.hpp"
#include <string>
#include <unordered_map>

namespace ESM {

struct Record;

class ESM {
public:
    ESM(std::string& path);
    ~ESM();

private:
    ESMReader                             reader;
    std::unordered_map<uint32_t, Record*> records;
};

};
