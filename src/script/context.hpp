#pragma once

//#include "../gameworld.hpp"
#include "types/base.hpp"
#include <string>
#include <unordered_map>
#include <utility>

namespace Script {

enum class VariableScope : uint8_t {
    Local,
    Global
};

struct VariableInfo {
    Type          type;
    VariableScope scope;
    uint16_t      index;
};

class Context {
public:
    bool varExists(std::string& var)
    {
        return variables.count(var);
    }

    void declareVar(std::string& name, Type type, VariableScope scope)
    {
        log_debug("VarDeclare: %s, %d", name.c_str(), variableIndex);
        VariableInfo info = { type, scope, variableIndex++ };
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

    //    Context(GameWorld::GameWorld* w)
    Context()
        : variableIndex(1)
    //  , world(w)
    {
    }
    ~Context() {};

    uint16_t SCROLookup(std::string& editorId)
    {
        log_info("SCROLookup: %s", editorId.c_str());

        if (!varExists(editorId)) {

            declareVar(editorId, Type::Reference, VariableScope::Global);

            // TODO: save formids
            // This is just a dummy lookup used for testing the compiler
            //        formid ref = world->getByEditorID(editorId);
            uint32_t ref = 1;

            if (!ref) {
                log_fatal("No such editorId: %s", editorId.c_str());
                return 0;
            }
        }

        return variables[editorId].index;
    }

    std::string scriptName;

private:
    std::unordered_map<std::string, VariableInfo> variables;
    uint16_t                                      variableIndex;
    // GameWorld::GameWorld*                         world;
};

};
