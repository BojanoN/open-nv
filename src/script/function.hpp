#pragma once
#include <string>
#include <unordered_map>

namespace Script {
struct FunctionInfo {
    uint16_t    opcode;
    std::string name;
    std::string alias;
    std::string description;
};

class FunctionResolver {
public:
    void callFunction(uint16_t functionID)
    {
    }
    static std::unordered_map<std::string, FunctionInfo> functions;
};
}
