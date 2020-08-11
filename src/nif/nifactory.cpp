#include "nifactory.hpp"
#include <string>
class NiObject;

const std::unordered_map<std::string, NiObject* (*)(NifReader&)> NiFactory::creatorMap = createCreatorMap();