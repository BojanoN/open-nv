/*#include "gmst.cpp"

namespace ESM {

void MenuIcon::load(ESMReader& reader) {
  reader.readNextSubrecordHeader();
  reader.checkSubrecordHeader(ESM::Names.EDID);
  reader.readArraySubrecord<char>(editorId.c_str());

  reader.readNextSubrecordHeader();
  reader.checkSubrecordHeader(ESM::Names.ICON);
  reader.readArraySubrecord<char>(largeIconFilename.c_str());

  reader.readNextSubrecordHeader();
  reader.checkSubrecordHeader(ESM::Names.MICO);
  reader.readArraySubrecord<char>(smallIconFilename.c_str());
};*/