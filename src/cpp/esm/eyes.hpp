#include "reader.hpp"
#include "record.hpp"
#include "structs.hpp"

namespace ESM {


struct Eyes : public Record {

	std::string editorId;
	std::string name;
	std::string texture;
	uint8_t flags;

	virtual void load(ESMReader& reader);
};


};