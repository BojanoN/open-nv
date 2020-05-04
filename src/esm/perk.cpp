#include "perk.hpp"
#include "reader.hpp"

namespace ESM {

Perk::Perk(ESMReader& reader)
    : Record(reader)
{

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    bool effectData      = false;
    bool effectCondition = false;

    while (reader.hasMoreSubrecords()) {

        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::FULL:
            reader.readStringSubrecord(this->name);
            break;
        case ESMType::DESC:
            reader.readStringSubrecord(this->description);
            break;
        case ESMType::ICON:
            reader.readStringSubrecord(this->largeIconFilename);
            break;
        case ESMType::MICO:
            reader.readStringSubrecord(this->smallIconFilename);
            break;
        case ESMType::CTDA: {
            if (!effectCondition) {
                Condition::load(reader, this->condition);
                effectCondition = true;
            } else {
                PerkEffect& tmp = this->effects.emplace_back();
                tmp.perkConditions.emplace_back();
                Condition::load(reader, tmp.perkConditions.back().condition);
            }
            break;
        }
        case ESMType::DATA: {
            if (!effectData) {
                reader.readSubrecord(this->data);
                effectData = true;
            } else {
                PerkEffect& tmp  = this->effects.back();
                uint8_t     type = static_cast<std::underlying_type<PerkEffectType>::type>(tmp.header.type);
                switch (type) {
                case 0:
                    tmp.data.emplace<0>();
                    reader.readSubrecord(std::get<0>(tmp.data));
                    break;
                case 1:
                    tmp.data.emplace<1>();
                    reader.readSubrecord(std::get<1>(tmp.data));
                    break;
                case 2:
                    tmp.data.emplace<2>();
                    reader.readSubrecord(std::get<2>(tmp.data));
                    break;
                default:
                    reader.reportError("Invalid perk data type");
                }
            }
            break;
        }
        case ESMType::PRKE:
            this->effects.emplace_back();
            reader.readSubrecord(this->effects.back().header);
            break;
        case ESMType::PRKC: {
            PerkEffect& tmp = this->effects.emplace_back();
            tmp.perkConditions.emplace_back();

            reader.readSubrecord(tmp.perkConditions.back().runOn);
            reader.readNextSubrecordHeader();
            reader.checkSubrecordHeader(ESMType::CTDA);

            Condition::load(reader, tmp.perkConditions.back().condition);
            break;
        }
        case ESMType::SCDA:
        case ESMType::SCHR:
            ScriptData::load(reader, this->effects.back().scripts);
            break;
        case ESMType::EPFT:
            reader.readSubrecord(this->effects.back().epfdType);
            break;
        case ESMType::EPFD: {
            PerkEffect& tmp  = this->effects.emplace_back();
            uint8_t     type = tmp.epfdType;
            if (type > 5) {
                reader.reportError("Invalid EntryPoint data type");
            }
            switch (type) {
            case 0:
                tmp.entryPointData.emplace<0>();
                reader.readArraySubrecord(std::get<0>(tmp.entryPointData));
                break;
            case 1:
                tmp.entryPointData.emplace<1>();
                reader.readSubrecord(std::get<1>(tmp.entryPointData));
                break;
            case 2:
                tmp.entryPointData.emplace<2>();
                reader.readSubrecord(std::get<2>(tmp.entryPointData));
                break;
            case 3:
                tmp.entryPointData.emplace<3>();
                reader.readSubrecord(std::get<3>(tmp.entryPointData));
                break;
            case 4:
                reader.skipSubrecord();
                break;
            case 5:
                tmp.entryPointData.emplace<5>();
                reader.readSubrecord(std::get<5>(tmp.entryPointData));
                break;
            default:
                reader.reportError("Invalid EntryPoint data type");
            }
            break;
        }
        case ESMType::EPF2:
            reader.readStringSubrecord(this->effects.back().buttonLabel);
            break;
        case ESMType::EPF3:
            reader.readSubrecord(this->effects.back().flags);
            break;
        case ESMType::PRKF:
            reader.skipSubrecord();
            break;
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
}
