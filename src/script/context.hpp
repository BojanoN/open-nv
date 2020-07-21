#pragma once

#include "../esm/records.hpp"
#include "../esm/types.hpp"
#include "../gamedata.hpp"
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

    Context(GameWorld::GameData<ESM::Script>* store)
        : variableIndex(0)
        , scriptStore(store)
    {
    }
    ~Context() {};

    uint16_t SCROLookup(std::string& editorId)
    {
        log_info("SCROLookup: %s", editorId.c_str());

        // TODO: save formids
        formid ref;

        try {
            ref = this->scriptStore->get(editorId);
        } catch (std::runtime_error& e) {
            log_fatal("No such editorId: %s", editorId.c_str());
            return 0;
        }
        return variableIndex++;
    }

    std::string scriptName;

private:
    std::unordered_map<std::string, VariableInfo> variables;
    uint16_t                                      variableIndex;
    GameWorld::GameData<ESM::Script>*             scriptStore;
};

};
