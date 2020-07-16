#pragma once

#include "types/base.hpp"
#include <string>
#include <unordered_map>
#include <utility>

namespace Script {

struct VariableInfo {
    Type     type;
    uint16_t index;
};

class Context {
public:
    bool varExists(std::string& var)
    {
        return variables.count(var);
    }

    void declareVar(std::string& name, Type type)
    {
        log_debug("VarDeclare: %s, %d", name.c_str(), variableIndex);
        VariableInfo info = { type, variableIndex++ };
        variables[name]   = info;
    }

    std::pair<VariableInfo, bool> getVar(std::string& name)
    {
        std::pair<VariableInfo, bool> ret;
        ret.second = false;

        if (variables.count(name)) {
            ret.second = true;
            ret.first  = variables[name];
        }

        return ret;
    }

    Context()
        : variableIndex(0)
    {
    }
    ~Context() {};

    uint32_t SCROLookup(std::string& formid)
    {
        log_info("SCROLookup: %s", formid.c_str());
        return 0;
    }

    std::string scriptName;

private:
    std::unordered_map<std::string, VariableInfo> variables;
    uint16_t                                      variableIndex;
};

};
