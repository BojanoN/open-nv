#include "scriptdata.hpp"

namespace ESM {

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

}; // namespace ESM