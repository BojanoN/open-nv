#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>

#include "../util/octet_hash.hpp"
#include "data_file.hpp"
#include "spdlog/spdlog.h"

const uint32_t group_hash = fnv_hash("GRUP", 4);

class ESM {
public:
    ESM(const char* path)
        : name(path),
          records(new std::unordered_map<uint32_t, Record*>()),
          groups(new std::unordered_map<uint32_t, Group*>())
    {
        std::ifstream file{path, std::ios::binary};
        uint8_t type[5] = {0};

        uint64_t group_count, record_count;

        while (file.read(reinterpret_cast<char*>(&type), 4)) {
            file.seekg(-4, std::ios_base::cur); // move 4 chars backwards
            uint32_t hash_val = fnv_hash(type, 4);

            if (hash_val == group_hash) {
                group_count++;
                spdlog::trace("Loading group");

                Group* tmp_g = new Group(file);
                groups->insert({tmp_g->ID, tmp_g});
                break;
            }
            else {
                // Load record
                record_count++;
                spdlog::trace("Loading record");
                Record* tmp_r;
                try{
                  Record::ParseRecord(file, hash_val);
                }catch (const std::runtime_error& e){
                  spdlog::warn("Record {}: {}", type, e.what());
                }
                records->insert({tmp_r->ID, tmp_r});
                break;
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
}
;
