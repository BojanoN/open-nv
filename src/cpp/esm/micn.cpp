#include "gmst.cpp"

namespace ESM {

void MenuIcon::load(ESMReader& reader) {
  reader.readNextSubrecordHeader();
  reader.checkSubrecordHeader(ESMType.EDID);
  reader.readArraySubrecord<char>(editorId.c_str());

  reader.readNextSubrecordHeader();
  reader.checkSubrecordHeader(ESMType.ICON);
  reader.readArraySubrecord<char>(largeIconFilename.c_str());

  reader.readNextSubrecordHeader();
  reader.checkSubrecordHeader(ESMType.MICO);
  reader.readArraySubrecord<char>(smallIconFilename.c_str());
};