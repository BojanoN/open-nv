#pragma once

#include <unordered_map>


class NifReader;
class NiObject;

struct NiFactory {
private:
    static std::unordered_map<std::string, std::shared_ptr<NiObject* (*)(NifReader&)>> createCreatorMap();
public:
    static const std::unordered_map<std::string, std::shared_ptr<NiObject* ()(NifReader&)>> creatorMap;
};

//const std::unordered_map<std::string, NiObject* (*)(NifReader&)> NiFactory::creatorMap = createCreatorMap();
