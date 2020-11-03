#pragma once
#include "record.hpp"
#include "subrecords/scriptdata.hpp"
#include "subrecords/condition.hpp"
#include "structs.hpp"

namespace ESM {

enum class QuestFlags : uint8_t {
    StartGameEnabled                = 0x01,
    Unknown                         = 0x02,
    AllowRepeatedConversationTopics = 0x04,
    AllowRepeatedStages             = 0x08,
    Unknown_2                       = 0x10
};

//Sometimes with no delay: size 2.
struct QuestData {
    QuestFlags flags;
    uint8_t    priority;
    uint8_t    padding[2];
    float      delay;
};

enum class QuestStageFlags : uint8_t {
    CompleteQuest = 0x01,
    FailQuest     = 0x02
};

struct QuestLogEntry {
    QuestStageFlags        flags;
    std::vector<Condition> conditions;
    std::string            logEntry;
    ScriptData             stageScript;
    formid                 nextQuest; //QUST

    static void load(ESMReader& reader, QuestLogEntry& entry);
};

struct QuestStage {
    int16_t                    stageIndex;
    std::vector<QuestLogEntry> logEntries;

    static void load(ESMReader& reader, QuestStage& stage);
};

enum class QuestObjectiveTargetFlags : uint8_t {
    CompassMarkerIgnoresLocks = 0x01
};

struct QuestObjectiveTargetData {
    formid                    target; //REFR, PGRE, PMIS, ACRE, ACHR
    QuestObjectiveTargetFlags flags;
    uint8_t                   padding[3];
};

struct QuestObjectiveTarget {
    QuestObjectiveTargetData data;
    std::vector<Condition>   conditions;
};

struct QuestObjective {
    int32_t                           objectiveIndex;
    std::string                       description;
    std::vector<QuestObjectiveTarget> targets;

    static void load(ESMReader& reader, QuestObjective& objective);
};

struct Quest : public ScriptableRecord {
    formid                      script;
    std::string                 name;
    std::string                 largeIconFilename;
    std::string                 smallIconFilename;
    QuestData                   data;
    std::vector<Condition>      conditions; //one 20 byte condition exists at 0xe755a0e
    std::vector<QuestStage>     stages;
    std::vector<QuestObjective> objectives;

    virtual formid getLinkedScript() { return script; };

    Quest(ESMReader& reader);
};
}
