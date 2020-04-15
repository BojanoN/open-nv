#pragma once
#include "reader.hpp"
#include "record.hpp"

namespace ESM {

struct MenuIcon : public Record {

	std::string editorId;
	std::string largeIconFilename;
	std::string smallIconFilename;

	virtual void load(ESMReader& reader);
};


};