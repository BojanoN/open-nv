#pragma once

#include <unordered_map>

//#include "nifdata.hpp"
#include "main/main.hpp"


class NifReader;

struct NiFactory {
private:
    static std::unordered_map<std::string, NiObject* (*)(NifReader&)> createCreatorMap() {
        std::unordered_map<std::string, NiObject* (*)(NifReader&)> m;
		m["NiObject"] = &NiObject::create;
		m["NiAVObject"] = &NiAVObject::create;
		m["NiDynamicEffect"] = &NiDynamicEffect::create;
		m["NiProperty"] = &NiProperty::create;
		m["NiObjectNET"] = &NiObjectNET::create;
		m["NiCollisionObject"] = &NiCollisionObject::create;
        return m;
    }
public:
    static const std::unordered_map<std::string, NiObject* (*)(NifReader&)> creatorMap;
};

//const std::unordered_map<std::string, NiObject* (*)(NifReader&)> NiFactory::creatorMap = createCreatorMap();
