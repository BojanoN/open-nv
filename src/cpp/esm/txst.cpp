/*#include "gmst.cpp"

namespace ESM {

void TextureSet::load(ESMReader& reader) {
  reader.readNextSubrecordHeader();
  reader.checkSubrecordHeader(ESM::Names.EDID);
  reader.readArraySubrecord<char>(editorId.c_str());

  reader.readNextSubrecordHeader();
  reader.checkSubrecordHeader(ESM::Names.OBND);
  reader.readSubrecord<ObjectBounds>(objectBounds);

  while (reader.subrecordType().intValue != ESM::Names.DNAM) {
    reader.readNextSubrecordHeader();
    switch (reader.subrecordType().intValue) {
      case ESM::Names.TX00:
        reader.readArraySubrecord<char>(baseImage_transparency.c_str());
        break;
      case ESM::Names.TX01:
        reader.readArraySubrecord<char>(normalMap_specular.c_str());
        break;
      case ESM::Names.TX02:
        reader.readArraySubrecord<char>(environmentMapMask.c_str());
        break;
      case ESM::Names.TX03:
        reader.readArraySubrecord<char>(glowMap.c_str());
        break;
      case ESM::Names.TX04:
        reader.readArraySubrecord<char>(parallaxMap.c_str());
        break;
      case ESM::Names.TX05:
        reader.readArraySubrecord<char>(environmentMap.c_str());
        break;
      case ESM::Names.DODT:
        reader.readSubrecord<DecalData>(decalData);
        break;
      default:
        break;
    }
  }

  reader.checkSubrecordHeader(ESM::Names.DNAM);
  reader.readSubrecord<uint16_t>(flags);
};*/