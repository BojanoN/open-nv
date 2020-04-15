#include "txst.hpp"

namespace ESM {

void TextureSet::load(ESMReader& reader) {
  reader.readNextSubrecordHeader();
  reader.checkSubrecordHeader(ESMType::EDID);
  reader.readStringSubrecord(editorId);

  while (reader.subrecordType() != ESMType::DNAM) {
    reader.readNextSubrecordHeader();
    switch (reader.subrecordType()) {
      case ESMType::OBND:
        reader.readSubrecord(objectBounds);
      case ESMType::TX00:
        reader.readStringSubrecord(baseImage_transparency);
        break;
      case ESMType::TX01:
        reader.readStringSubrecord(normalMap_specular);
        break;
      case ESMType::TX02:
        reader.readStringSubrecord(environmentMapMask);
        break;
      case ESMType::TX03:
        reader.readStringSubrecord(glowMap);
        break;
      case ESMType::TX04:
        reader.readStringSubrecord(parallaxMap);
        break;
      case ESMType::TX05:
        reader.readStringSubrecord(environmentMap);
        break;
      case ESMType::DODT:
        reader.readSubrecord(decalData);
        break;
      default:
        break;
    }
  }

  reader.checkSubrecordHeader(ESMType::DNAM);
  reader.readSubrecord(flags);

}

};