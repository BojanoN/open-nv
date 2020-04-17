#include "tact.hpp"

namespace ESM {

TalkingActivator::TalkingActivator(ESMReader& reader) : Record(reader) {
  reader.readNextSubrecordHeader();
  reader.checkSubrecordHeader(ESMType::EDID);
  reader.readStringSubrecord(editorId);

  std::unordered_set<ESMType> nextSubheadersModel{
      ESMType::SCRI, ESMType::DEST, ESMType::SNAM, ESMType::VNAM,
      ESMType::INAM, ESMType::RNAM, ESMType::WNAM, ESMType::XATO};
  std::unordered_set<ESMType> nextSubheadersDest{ESMType::SNAM, ESMType::VNAM,
                                                 ESMType::INAM, ESMType::RNAM,
                                                 ESMType::WNAM, ESMType::XATO};

  while (reader.hasMoreSubrecords()) {
  	reader.readNextSubrecordHeader();
    switch (reader.subrecordType()) {
      case ESMType::OBND:
      	reader.readSubrecord(objectBounds);
      	break;
      case ESMType::FULL:
        reader.readStringSubrecord(name);
        break;
      case ESMType::MODL:
        ModelData::load(reader, modelData, 0, nextSubheadersModel);
        break;
      case ESMType::SCRI:
        reader.readSubrecord(script);
        break;
      case ESMType::DEST:
        DestructionData::load(reader, destructionData, nextSubheadersDest);
        break;
      case ESMType::SNAM:
        reader.readSubrecord(soundLooping);
        break;
      case ESMType::VNAM:
        reader.readSubrecord(voiceType);
        break;
      case ESMType::INAM:
        reader.readSubrecord(radioTemplate);
        break;
    }
  }
}
};