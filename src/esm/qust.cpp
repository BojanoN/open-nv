#include "qust.hpp"

namespace ESM {

void QuestLogEntry::load(ESMReader& reader, QuestLogEntry& entry)
{
    while (reader.peekNextType() != ESMType::SCHR) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::QSDT:
            reader.readSubrecord(entry.flags);
            break;
        case ESMType::CTDA:
            entry.conditions.emplace_back();
            Condition::load(reader, entry.conditions.back());
            //reader.readSubrecord(entry.conditions.back());
            break;
        case ESMType::CNAM:
            reader.readStringSubrecord(entry.logEntry);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
    ScriptData::load(reader, entry.stageScript);
    if (reader.peekNextType() == ESMType::NAM0) {
        reader.readNextSubrecordHeader();
        reader.readSubrecord(entry.nextQuest);
    }
}

void QuestStage::load(ESMReader& reader, QuestStage& stage)
{
    reader.readSubrecord(stage.stageIndex);
    while (reader.hasMoreSubrecords() && reader.peekNextType() != ESMType::QOBJ && reader.peekNextType() != ESMType::NNAM && reader.peekNextType() != ESMType::INDX) {
        stage.logEntries.emplace_back();
        QuestLogEntry::load(reader, stage.logEntries.back());
    }
}

void QuestObjective::load(ESMReader& reader, QuestObjective& objective)
{
    reader.readSubrecord(objective.objectiveIndex);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::NNAM);
    reader.readStringSubrecord(objective.description);
    while (reader.peekNextType() == ESMType::QSTA) {
        reader.readNextSubrecordHeader();
        objective.targets.emplace_back();
        reader.readSubrecord(objective.targets.back().data);
        while (reader.peekNextType() == ESMType::CTDA) {
            reader.readNextSubrecordHeader();
            objective.targets.back().conditions.emplace_back();
            Condition::load(reader, objective.targets.back().conditions.back());
            //reader.readSubrecord(objective.targets.back().conditions.back());
        }
    }
}

Quest::Quest(ESMReader& reader)
    : ScriptableRecord(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::SCRI:
            reader.readSubrecord(script);
            break;
        case ESMType::FULL:
            reader.readStringSubrecord(name);
            break;
        case ESMType::ICON:
            reader.readStringSubrecord(largeIconFilename);
            break;
        case ESMType::MICO:
            reader.readStringSubrecord(smallIconFilename);
            break;
        case ESMType::DATA:
            reader.readSubrecord(data);
            break;
        case ESMType::CTDA:
            conditions.emplace_back();
            Condition::load(reader, conditions.back());
            //reader.readSubrecord(conditions.back());
            break;
        case ESMType::INDX:
            stages.emplace_back();
            QuestStage::load(reader, stages.back());
            break;
        case ESMType::QOBJ:
            objectives.emplace_back();
            QuestObjective::load(reader, objectives.back());
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
};
