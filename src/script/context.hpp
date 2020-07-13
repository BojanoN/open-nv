#pragma once

#include "types/base.hpp"
#include <string>
#include <unordered_map>
#include <utility>

namespace Script {

class Context {
public:
    bool varExists(std::string& var)
    {
        return variables.count(var);
    }

    void declareVar(std::string& name, Type type)
    {
        variables[name] = std::make_pair(type, variableIndex);
        variableIndex++;
    }

    std::pair<Type, int>& getVar(std::string& name)
    {
        return variables[name];
    }
    Context()
        : variableIndex(0)
    {
    }
    ~Context() {};

    int SCROLookup(std::string& formid)
    {
        log_info("SCROLookup: %s", formid.c_str());
        return 0;
    }

    std::string scriptName;

private:
    std::unordered_map<std::string, std::pair<Type, int>> variables;
    int                                                   variableIndex;
};

};
