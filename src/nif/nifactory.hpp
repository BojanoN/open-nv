<<<<<<< a3c8b8e7d299c1e76723a2121f70dd14fb2ab692
#pragma once

#include <unordered_map>


class NifReader;
class NiObject;

struct NiFactory {
private:
    static std::unordered_map<std::string, NiObject* (*)(NifReader&)> createCreatorMap();
public:
    static const std::unordered_map<std::string, NiObject* (*)(NifReader&)> creatorMap;
};

//const std::unordered_map<std::string, NiObject* (*)(NifReader&)> NiFactory::creatorMap = createCreatorMap();
=======

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
>>>>>>> Begin nif reading code, refactor generator
