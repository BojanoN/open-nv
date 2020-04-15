#include "gmst.cpp"

namespace ESM {

void TextureSet::load(ESMReader& reader) {
  reader.readNextSubrecordHeader();
  reader.checkSubrecordHeader(ESMType.EDID);
  reader.readArraySubrecord<char>(editorId.c_str());

  reader.readNextSubrecordHeader();
  reader.checkSubrecordHeader(ESMType.OBND);
  reader.readSubrecord<ObjectBounds>(objectBounds);

  while (reader.subrecordType().intValue != ESMType.DNAM) {
    reader.readNextSubrecordHeader();
    switch (reader.subrecordType().intValue) {
      case ESMType.TX00:
        reader.readArraySubrecord<char>(baseImage_transparency.c_str());
        break;
      case ESMType.TX01:
        reader.readArraySubrecord<char>(normalMap_specular.c_str());
        break;
      case ESMType.TX02:
        reader.readArraySubrecord<char>(environmentMapMask.c_str());
        break;
      case ESMType.TX03:
        reader.readArraySubrecord<char>(glowMap.c_str());
        break;
      case ESMType.TX04:
        reader.readArraySubrecord<char>(parallaxMap.c_str());
        break;
      case ESMType.TX05:
        reader.readArraySubrecord<char>(environmentMap.c_str());
        break;
      case ESMType.DODT:
        reader.readSubrecord<DecalData>(decalData);
        break;
      default:
        break;
    }
  }

  reader.checkSubrecordHeader(ESMType.DNAM);
  reader.readSubrecord<uint16_t>(flags);
};