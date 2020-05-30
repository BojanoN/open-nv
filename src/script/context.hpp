#pragma once

#include <unordered_map>

namespace Script {

class Object;

class Context {
public:
    bool varExists(std::string& var)
    {
        return variables.count(var);
    }

    void declareVar(std::string& name, Object* var)
    {
        variables[name] = var;
    }

    ~Context();

private:
    std::unordered_map<std::string, Object*> variables;
};

};
