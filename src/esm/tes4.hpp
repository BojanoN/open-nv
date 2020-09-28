#pragma once

#include "reader.hpp"
#include "record.hpp"
#include <iostream>

namespace ESM {

struct PluginInfoHeader {
	float version;
	uint32_t numRecords;
	uint32_t nextObjectId;
};


struct MasterPluginInfo : public Record {

	PluginInfoHeader header;
	std::string author;
	std::string description;
	std::vector<std::string> parentMasters;
	
	MasterPluginInfo(ESMReader& reader);
    ~MasterPluginInfo() {};
 	
};

}; // namespace ESM
