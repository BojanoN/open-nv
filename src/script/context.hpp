#pragma once

#include <unordered_map>

namespace Script {

class Context {
public:
    bool exists(std::string& var)
    {
        return variables.count(var);
    }

private:
    std::unordered_map<std::string, int> variables;
};

};
