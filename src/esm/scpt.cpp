#include "scpt.hpp"
#include "reader.hpp"

namespace ESM {

Script::Script(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::SCHR);
    reader.readSubrecord(this->scriptHeader);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::SCDA);
    reader.readArraySubrecord(this->compiledSource);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::SCTX);
    reader.readStringSubrecord(this->scriptSource);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::SCRO:
            this->references.emplace_back();
            this->references.back().type = ReferenceType::OBJECT_REFERENCE;
            reader.readSubrecord(this->references.back().reference);
            break;
        case ESMType::SCRV:
            this->references.emplace_back();
            this->references.back().type = ReferenceType::VARIABLE_REFERENCE;
            reader.readSubrecord(this->references.back().reference);
            break;
        case ESMType::SLSD:
            this->localVariables.emplace_back();
            reader.readSubrecord(this->localVariables.back().data);
            break;
        case ESMType::SCVR:
            reader.readStringSubrecord(this->localVariables.back().name);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}

};
