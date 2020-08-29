#include "structs.hpp"
#include "util/strmanip.hpp"

namespace ESM {

bool DestructionData::isInCollection(uint32_t type)
{
    for (int i = 0; i < 4; i++) {
        if (types[i] == type) {
            return true;
        }
    }
    return false;
}

void DestructionData::load(ESMReader& reader, DestructionData& destData)
{
    reader.checkSubrecordHeader(ESMType::DEST);
    reader.readSubrecord(destData.header);

    while (reader.hasMoreSubrecords() && isInCollection(reader.peekNextType())) {
        reader.readNextSubrecordHeader();
        reader.checkSubrecordHeader(ESMType::DSTD);

        destData.stages.emplace_back();
        reader.readSubrecord(destData.stages.back().stageData);
        bool stageEnd = false;

        while (reader.hasMoreSubrecords() && isInCollection(reader.peekNextType()) && !stageEnd) {

            reader.readNextSubrecordHeader();
            if (reader.subrecordType() == ESMType::DMDL) {
                reader.readStringSubrecord(destData.stages.back().modelFilename);

            } else if (reader.subrecordType() == ESMType::DMDT) {
                reader.readArraySubrecord(destData.stages.back().textureHashes);
            } else if (reader.subrecordType() == ESMType::DSTF) {
                stageEnd = true;
            }
        }
    }
}

void Condition::load(ESMReader& reader, Condition& condition)
{
    if (reader.getCurrentSubrecord().dataSize == 28) {
        reader.readSubrecord(*reinterpret_cast<Condition::ConditionNullReference*>(&condition));
    } else {
        reader.readSubrecord(condition);
    }
}

void ArmorAttributes::load(ESMReader& reader, ArmorAttributes& attrs)
{
    if (reader.getCurrentSubrecord().dataSize == 4) {
        reader.readSubrecord(*reinterpret_cast<ArmorAttributes::ArmorAttributesSmall*>(&attrs));
    } else {
        reader.readSubrecord(attrs);
    }
}

void ScriptData::load(ESMReader& reader, ScriptData& scriptData)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::SCHR);
    reader.readSubrecord(scriptData.scriptHeader);

    while (reader.hasMoreSubrecords()) {
        ESMType next = reader.peekNextType();

        if (!(next == ESMType::SCRO || next == ESMType::SCRV || next == ESMType::SLSD || next == ESMType::SCVR || next == ESMType::SCDA || next == ESMType::SCTX))
            break;

        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::SCTX:
            reader.readStringSubrecord(scriptData.scriptSource);
            break;
        case ESMType::SCDA:
            reader.readArraySubrecord(scriptData.compiledSource);
            break;
        case ESMType::SCRO:
            scriptData.references.emplace_back();
            scriptData.references.back().type = ReferenceType::OBJECT_REFERENCE;
            reader.readSubrecord(scriptData.references.back().reference);
            break;
        case ESMType::SCRV:
            scriptData.references.emplace_back();
            scriptData.references.back().type = ReferenceType::VARIABLE_REFERENCE;
            reader.readSubrecord(scriptData.references.back().reference);
            break;
        case ESMType::SLSD:
            scriptData.localVariables.emplace_back();
            reader.readSubrecord(scriptData.localVariables.back().data);
            break;
        case ESMType::SCVR: {
            std::string& name = scriptData.localVariables.back().name;
            reader.readStringSubrecord(name);
            ::Util::toLowerCase(name);
        } break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
};
