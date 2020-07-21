#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

struct SharedInfo {
    formid  infoConnection; //INFO
    int32_t infoIndex;
};

struct AddedQuest {
    formid  quest;  //QUST
    std::vector<SharedInfo> sharedInfos;

    static void load(AddedQuest& addedQuest, ESMReader& reader);
};

enum class DialogueTopicType : uint8_t {
    TOPIC,
    CONVERSATION,
    COMBAT,
    PERSUASION,
    DETECTION,
    SERVICE,
    MISCELLANEOUS,
    RADIO
};

enum class DialogueTopicFlags : uint8_t {
    RUMORS = 0x01,
    TOP_LEVEL = 0x02
};

struct DialogueTopicData {
    DialogueTopicType   type;
    DialogueTopicFlags  flags;
};


struct DialogueTopic : public Record {
    std::vector<AddedQuest> addedQuests;
    std::vector<formid>     removedQuests;
    //unused collection of INFC and INFX subrecords (like SharedInfo)
    //"probably a GECK bug"
    std::string             name;
    float                   priority;
    std::string             tdum; //unknown purpose
    DialogueTopicData       data;


    DialogueTopic(ESMReader& reader);
};

}
