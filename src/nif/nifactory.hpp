#pragma once

#include <string>
#include <unordered_map>

class NifReader;
struct NiObject;

struct NiFactory {
private:
    static std::unordered_map<std::string, NiObject* (*)(NifReader&)> createCreatorMap();

public:
    static const std::unordered_map<std::string, NiObject* (*)(NifReader&)> creatorMap;
};

//const std::unordered_map<std::string, NiObject* (*)(NifReader&)> NiFactory::creatorMap = createCreatorMap();
