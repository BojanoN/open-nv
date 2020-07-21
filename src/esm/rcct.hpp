#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

enum class RecipeCategoryFlags : uint8_t {
	IsSubcategory = 0x01
};

struct RecipeCategory : public Record {
    std::string     name;
    RecipeCategoryFlags flags;

    RecipeCategory(ESMReader& reader);

};
}