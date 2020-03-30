#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>

#include "data_file.hpp"
#include "spdlog/spdlog.h"

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

        uint64_t group_count, record_count;

        while (file.read(reinterpret_cast<char*>(&type), 4)) {
            switch (type) {
                // Load groups
            case (GRUP): {
                group_count++;
                spdlog::trace("Loading group");

                file.seekg(-4, std::ios_base::cur); // move 4 chars backwards
                Group* tmp_g = new Group(file);
                groups->insert({tmp_g->ID, tmp_g});
                break;
            }
                // Load record
            default: {
                record_count++;
                spdlog::trace("Loading record");

                file.seekg(-4, std::ios_base::cur); // move 4 chars backwards
                Record* tmp_r = new Record(file);
                records->insert({tmp_r->ID, tmp_r});
                break;
            }
            }
        }
        spdlog::info("Loaded {} groups and {} records", group_count, record_count);
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
