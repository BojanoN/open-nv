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
    static bool callFunction(uint16_t functionID);
    static bool lookupFunction(uint16_t functionID);

    // TODO: this is just a dummy implemetation.
    // When the engine matures enough there should be an interface for all functions.
    // The lookup function should return a function pointer/handle to avoid redundant calls
    static std::unordered_map<std::string, FunctionInfo> functions;

private:
    static std::unordered_map<uint16_t, std::string> opcodeFuncs;
};
}
