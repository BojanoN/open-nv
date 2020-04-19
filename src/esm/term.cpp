#include "term.hpp"

namespace ESM {

Terminal::Terminal(ESMReader& reader) : Record(reader) {
  reader.readNextSubrecordHeader();
  reader.checkSubrecordHeader(ESMType::EDID);
  reader.readStringSubrecord(editorId);

  std::unordered_set<ESMType> nextSubheadersModel{
      ESMType::SCRI, ESMType::DEST, ESMType::DESC};
  std::unordered_set<ESMType> nextSubheadersDest{ESMType::DESC};

  while (reader.hasMoreSubrecords() && reader.subrecordType() != ESMType::DESC) {
  	reader.readNextSubrecordHeader();
    switch (reader.subrecordType()) {
      case ESMType::OBND:
      	reader.readSubrecord(objectBounds);
      	break;
      case ESMType::FULL:
        reader.readStringSubrecord(name);
        break;
      case ESMType::MODL:
        ModelData::load(reader, modelData, 0);
        break;
      case ESMType::SCRI:
        reader.readSubrecord(script);
        break;
      case ESMType::DEST:
        DestructionData::load(reader, destructionData);
        break;
       default:
       	 break;

       }
   }

    //reader.readNextSubrecordHeader();
  	reader.checkSubrecordHeader(ESMType::DESC);
  	reader.readStringSubrecord(description);

  	reader.readNextSubrecordHeader();
  	if(reader.subrecordType() == ESMType::SNAM) {
  		reader.readSubrecord(soundLooping);
  		reader.readNextSubrecordHeader();
  	} 

  	if (reader.subrecordType() == ESMType::PNAM) {
  		reader.readSubrecord(passwordNote);
  		reader.readNextSubrecordHeader();
  	}

  	reader.checkSubrecordHeader(ESMType::DNAM);
  	if(reader.getCurrentSubrecord().dataSize == 3) {
  		reader.readSubrecord(*reinterpret_cast<TerminalData::TerminalDataPacked*>(&data));
  	} else {
  		reader.readSubrecord(data);
  	}

  	while(reader.hasMoreSubrecords()) {
  		reader.readNextSubrecordHeader();
  		menuItems.emplace_back();
  		if(reader.subrecordType() == ESMType::ITXT) {
  			reader.readStringSubrecord(menuItems.back().itemText);
  			reader.readNextSubrecordHeader();
  		}

  		if(reader.subrecordType() == ESMType::RNAM) {
	  		reader.readStringSubrecord(menuItems.back().resultText);
	  		reader.readNextSubrecordHeader();

  		}
  		reader.checkSubrecordHeader(ESMType::ANAM);
  		reader.readSubrecord(menuItems.back().flags);

  		if (reader.peekNextType() == ESMType::INAM) {
  			reader.readNextSubrecordHeader();
  			reader.readSubrecord(menuItems.back().displayNote);
  			//reader.readNextSubrecordHeader();
  		}
  		if (reader.peekNextType() == ESMType::TNAM) {
  			reader.readNextSubrecordHeader();
  			reader.readSubrecord(menuItems.back().subMenu);
  			//reader.readNextSubrecordHeader();
  		}

  		std::unordered_set<ESMType> scriptSubrecs{
  			ESMType::SCHR, ESMType::SCDA, ESMType::SCTX,
  			ESMType::SLSD, ESMType::SCVR, ESMType::SCRO,
  			ESMType::SCRV};

  		while(scriptSubrecs.find(static_cast<ESMType>(reader.peekNextType())) != scriptSubrecs.end()) {
  			reader.readNextSubrecordHeader();
  			reader.skipSubrecord();
  		}
  		//Skipping embedded script...
  		/*reader.checkSubrecordHeader(ESMType::SCHR);
  		reader.skipSubrecord();
  		reader.readNextSubrecordHeader();
  		reader.checkSubrecordHeader(ESMType::SCDA);
  		reader.readNextSubrecordHeader();
  		reader.skipSubrecord();
  		reader.checkSubrecordHeader(ESMType::SCTX);
  		reader.readNextSubrecordHeader();
  		reader.skipSubrecord();

  		while(reader.peekNextType() == ESMType::SLSD ||
  			reader.peekNextType() == ESMType::SCVR ||
  			reader.peekNextType() == ESMType::SCRO ||
  			reader.peekNextType() == ESMType::SCRV) {
  			reader.readNextSubrecordHeader();
  			reader.skipSubrecord();
  		}*/
  		//Skipping embedded script...

  		while(reader.peekNextType() == ESMType::CTDA) {
  			reader.readNextSubrecordHeader();
  			menuItems.back().conditions.emplace_back();
  			Condition::load(reader, menuItems.back().conditions.back());
  		}

  	}
}


};
