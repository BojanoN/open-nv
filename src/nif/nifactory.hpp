
#pragma once
#include "main/main.hpp"

#include <unordered_map>

struct NiFactory {
private:
    static std::unordered_map<std::string, NiObject* (*)(FILE*)> createCreatorMap() {
        std::unordered_map<std::string, NiObject* (*)(FILE*)> m;
		m["NiObject"] = &NiObject::create;
        return m;
    }
public:
    static const std::unordered_map<std::string, NiObject* (*)(FILE*)> creatorMap;
};

const std::unordered_map<std::string, NiObject* (*)(FILE*)> NiFunctionMaps::creatorMap = createCreatorMap();
