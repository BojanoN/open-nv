#pragma once
#include "record.hpp"
#include <string>

namespace ESM {

class ESMReader;

struct MenuIcon : public Record {

	std::string editorId;
	std::string largeIconFilename;
	std::string smallIconFilename;

	MenuIcon(ESMReader& reader);
};


};