#pragma once
#include <string>
#include <unordered_map>
#include <utility>

#include "types/base.hpp"

namespace Script {

enum class FunctionType {
    Standalone,
    ReferenceBound,
    Mixed
};

struct FunctionParameterInfo {
    // The defaultValue field also holds the parameter type
    Value defaultValue;
    bool  optional;
};

struct FunctionInfo {
    uint16_t                           opcode;
    std::string                        name;
    std::string                        description;
    FunctionType                       type;
    Type                               returnType;
    std::vector<FunctionParameterInfo> params;
};

class FunctionResolver {
public:
    static bool         callFunction(uint16_t functionID);
    static bool         lookupFunction(uint16_t functionID);
    static bool         isFunction(std::string& name);
    static bool         isFunctionAlias(std::string& name);
    static std::string& getFunctionByAlias(std::string& name);

    static FunctionInfo& getFunctionInfo(std::string& name);

    // TODO: this is just a dummy implemetation.
    // When the engine matures enough there should be an interface for all functions.
    // The lookup function should return a function pointer/handle to avoid redundant calls
private:
    static std::unordered_map<std::string, FunctionInfo> functions;
    static std::unordered_map<uint16_t, std::string>     opcodeFuncs;
    static std::unordered_map<std::string, std::string>  functionAliases;
};
}
