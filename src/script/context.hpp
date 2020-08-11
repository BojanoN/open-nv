#pragma once

#include "../gameworld.hpp"
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

    std::pair<VariableInfo, bool> getScriptLocalVar(std::string& scriptEditorId, std::string& variable)
    {
        std::pair<VariableInfo, bool> ret;
        uint32_t                      form;

        ret.second = false;

        form = this->world->getByEditorID(scriptEditorId);

        if (form) {
            return ret;
            std::vector<ESM::LocalVariable>& variables = this->scriptStore->get(form)->data.localVariables;
            uint32_t                         size      = variables.size();
            ESM::LocalVariableData           targetVarData;
            bool                             found = false;

            for (uint32_t i = 0; i < size; i++) {
                if (!strcmp(scriptEditorId.c_str(), variables[i].name.c_str())) {
                    found         = true;
                    targetVarData = variables[i].data;
                    break;
                }
            }

            if (!found) {
                log_fatal("Variable %s does not exists in script %s", variable.c_str(), scriptEditorId.c_str());
                return ret;
            }

            switch (targetVarData.type) {
            case (0):
                ret.first.type = Type::Float;
                break;
            case (1):
                ret.first.type = Type::Integer;
                break;
            default:
                log_warn("Unknown value 0x%x encountered in SLSD subrecord, assuming variable type is int...", targetVarData.type);
                ret.first.type = Type::Integer;
                break;
            }

            ret.first.index = targetVarData.index;
            ret.second      = true;
            return ret;
        } else {
            log_fatal("No scriptable record with editorId %s", scriptEditorId.c_str());
            return ret;
        }

        return ret;
    }

    Context(GameWorld::GameWorld* w)
        : variableIndex(1)
        , world(w)
    {
        this->scriptStore = (GameWorld::GameData<ESM::Script>*)w->getDataStore(ESM::ESMType::SCPT);
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
    GameWorld::GameWorld*                         world;
    GameWorld::GameData<ESM::Script>*             scriptStore;
};
};
