#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {


enum class MessageFlags : uint32_t {
	MESSAGE_BOX = 0x00000001,
	AUTO_DISPLAY = 0x00000002
};

struct MenuButton {
	std::string 		   buttonText;
	std::vector<Condition> conditions;

	static void load(ESMReader& reader, MenuButton& button);
};

struct Message : public Record {

    std::string  			description;
    std::string  			name;
    formid 		 			icon; //MICN
    MessageFlags 			flags;
    uint32_t     			displayTime;
    std::vector<MenuButton> menuButtons;

    Message(ESMReader& reader);
};
};