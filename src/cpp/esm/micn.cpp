#include "micn.hpp"

namespace ESM {

MenuIcon::MenuIcon(ESMReader& reader) : Record(reader) {
  reader.readNextSubrecordHeader();
  reader.checkSubrecordHeader(ESMType::EDID);
  reader.readStringSubrecord(editorId);

  reader.readNextSubrecordHeader();
  reader.checkSubrecordHeader(ESMType::ICON);
  reader.readStringSubrecord(largeIconFilename);

  if(reader.hasMoreSubrecords()) {
  	reader.readNextSubrecordHeader();
  	reader.checkSubrecordHeader(ESMType::MICO);
  	reader.readStringSubrecord(smallIconFilename);
  }
}

};