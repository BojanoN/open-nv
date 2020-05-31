#pragma once
#include <string>
#include <unordered_map>

namespace Script {
struct FunctionInfo {
    uint32_t    opcode;
    std::string name;
    std::string alias;
    std::string description;
};

class FunctionResolver {
public:
    static std::unordered_map<std::string, FunctionInfo> functions;
};
}
