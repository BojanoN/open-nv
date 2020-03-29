#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>

#include "data_file.hpp"

#define GRUP 0x47525550

class ESM {
public:
    ESM(const char* path)
        : name(path),
          records(new std::unordered_map<uint32_t, Record*>()),
          groups(new std::unordered_map<uint32_t, Group*>())
    {
        std::ifstream file{path, std::ios::binary};
        uint32_t type;

        while (file.read(reinterpret_cast<char*>(&type), 4)) {
#ifdef DEBUG
            break;
#endif
            switch (type) {
                // Load groups
            case (GRUP): {
                Group* tmp_g = new Group(file);
                groups->insert({tmp_g->ID, tmp_g});
                break;
            }
                // Load record
            default: {
                Record* tmp_r = new Record(file);
                records->insert({tmp_r->ID, tmp_r});
                break;
            }
            }
        }
    };

    ~ESM()
    {
        delete records;
        delete groups;
    };

    std::string name;

    std::unordered_map<uint32_t, Record*>* records;
    std::unordered_map<uint32_t, Group*>* groups;
};
