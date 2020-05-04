#include "csno.hpp"
#include "reader.hpp"

namespace ESM {

Casino::Casino(ESMReader& reader)
    : Record(reader)
{
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(this->editorId);

    uint32_t modlCount = 0;
    uint32_t iconCount = 0;
    uint32_t ico2Count = 0;

    while (reader.hasMoreSubrecords()) {
        reader.readNextSubrecordHeader();
        switch (reader.subrecordType()) {
        case ESMType::FULL:
            reader.readStringSubrecord(this->name);
            break;
        case ESMType::DATA:
            reader.readSubrecord(this->data);
            break;
        case ESMType::MODL: {
            switch (modlCount) {
            case 0:
                reader.readStringSubrecord(this->chipModel.OneDollarChip);
                break;
            case 1:
                reader.readStringSubrecord(this->chipModel.FiveDollarChip);
                break;
            case 2:
                reader.readStringSubrecord(this->chipModel.TenDollarChip);
                break;
            case 3:
                reader.readStringSubrecord(this->chipModel.TwentyFiveDollarChip);
                break;
            case 4:
                reader.readStringSubrecord(this->chipModel.HundredDollarChip);
                break;
            case 5:
                reader.readStringSubrecord(this->chipModel.FiveHundredDollarChip);
                break;
            case 6:
                reader.readStringSubrecord(this->chipModel.rouletteChipModel);
                break;
            case 7:
                reader.readStringSubrecord(this->chipModel.slotMachineModel);
                break;
            default:
                reader.reportError("Too many MODL entries while parsing CSNO");
            }
            modlCount++;
            break;
        }
        case ESMType::MOD2:
            reader.skipSubrecord();
            break;
        case ESMType::MOD3:
            reader.readStringSubrecord(this->blackjackTableModel);
            break;
        case ESMType::MOD4:
            reader.readStringSubrecord(this->rouletteTableModel);
            break;
        case ESMType::ICON: {
            switch (iconCount) {
            case 0:
                reader.readStringSubrecord(this->slotReelTexture.symbol1);
                break;
            case 1:
                reader.readStringSubrecord(this->slotReelTexture.symbol2);
                break;
            case 2:
                reader.readStringSubrecord(this->slotReelTexture.symbol3);
                break;
            case 3:
                reader.readStringSubrecord(this->slotReelTexture.symbol4);
                break;
            case 4:
                reader.readStringSubrecord(this->slotReelTexture.symbol5);
                break;
            case 5:
                reader.readStringSubrecord(this->slotReelTexture.symbol6);
                break;
            case 6:
                reader.readStringSubrecord(this->slotReelTexture.symbolW);
                break;
            default:
                reader.reportError("Too many ICON entries while parsing CSNO");
            }
            iconCount++;
            break;
        }
        case ESMType::ICO2: {
            switch (ico2Count) {
            case 0:
                reader.readStringSubrecord(this->blackjackTexture.deck1);
                break;
            case 1:
                reader.readStringSubrecord(this->blackjackTexture.deck2);
                break;
            case 2:
                reader.readStringSubrecord(this->blackjackTexture.deck3);
                break;
            case 3:
                reader.readStringSubrecord(this->blackjackTexture.deck4);
                break;
            default:
                reader.reportError("Too many ICO2 entries while parsing CSNO");
            }
            ico2Count++;
            break;
        }
        default:
            std::stringstream s;
            s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
            reader.reportError(s.str());
        }
    }
}
};
