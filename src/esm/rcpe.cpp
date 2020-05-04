#include "rcpe.hpp"

namespace ESM {

void Recipe::loadItem(ESMReader& reader, std::vector<RecipeItem>& items) {
    items.emplace_back();
    reader.readSubrecord(items.back().item);
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::RCQY);
    reader.readSubrecord(items.back().quantity);
}


Recipe::Recipe(ESMReader& reader)
    : Record(reader)
{

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::FULL:
            reader.readStringSubrecord(name);
            break;
        case ESMType::CTDA:
            conditions.emplace_back();
            Condition::load(reader, conditions.back());
            break;
        case ESMType::DATA:
            reader.readSubrecord(data);
            break;
        case ESMType::RCIL:
            loadItem(reader, ingredients);
            break;
        case ESMType::RCOD:
            loadItem(reader, outputs);
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
};