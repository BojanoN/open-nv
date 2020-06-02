#pragma once

#include "types/base.hpp"
#include <string>
#include <unordered_map>

namespace Script {

class Context {
public:
    bool varExists(std::string& var)
    {
        return variables.count(var);
    }

    void declareVar(std::string& name, Type type)
    {
        variables[name] = type;
    }

    ~Context();

private:
    std::unordered_map<std::string, Type> variables;
};

};
