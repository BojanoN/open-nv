#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct RecipeData {
	int32_t skill;
	uint32_t level;
	formid  category; //RCCT
	formid  subCategory; //RCCT
};

struct RecipeItem {
	formid item;
	uint32_t quantity;
};

struct Recipe : public Record {
    std::string     name;
    std::vector<Condition> conditions;
    RecipeData	data;
    std::vector<RecipeItem> ingredients;
    std::vector<RecipeItem> outputs;
    Recipe(ESMReader& reader);

private:
	void loadItem(ESMReader& reader, std::vector<RecipeItem>& items);
};
}
